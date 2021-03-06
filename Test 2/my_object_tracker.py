import time
import tensorflow as tf
import core.utils as utils
from core.yolov4 import filter_boxes
from tensorflow.python.saved_model import tag_constants
from core.config import cfg
from PIL import Image
import cv2
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.compat.v1 import ConfigProto
from tensorflow.compat.v1 import InteractiveSession
# deep sort imports
from deep_sort import preprocessing, nn_matching
from deep_sort.detection import Detection
from deep_sort.tracker import Tracker
from tools import generate_detections as gdet

class MyTracker:
    def __init__(self, filePath):
        self.weights = './checkpoints/yolov4-tiny-416'        
        self.tiny = True
        self.size = 416
        self.model = 'yolov4'
        self.iou = 0.45
        self.score = 0.50
        self.filePath = filePath
    
    def trackingInit(self):
        # Definition of the parameters
        max_cosine_distance = 0.4
        nn_budget = None
        self.nms_max_overlap = 1.0
    
        # initialize deep sort
        model_filename = 'model_data/mars-small128.pb'
        self.encoder = gdet.create_box_encoder(model_filename, batch_size=1)
        # calculate cosine distance metric
        metric = nn_matching.NearestNeighborDistanceMetric("cosine", max_cosine_distance, nn_budget)
        # initialize tracker
        self.tracker = Tracker(metric)

        # load configuration for object detector
        config = ConfigProto()
        config.gpu_options.allow_growth = True
        session = InteractiveSession(config=config)
    
        STRIDES, ANCHORS, NUM_CLASS, XYSCALE = utils.load_config(self)
        self.input_size = self.size
        video_path = self.filePath

        # otherwise load standard tensorflow saved model
        self.saved_model_loaded = tf.saved_model.load(self.weights, tags=[tag_constants.SERVING])

        # begin video capture
        try:
            self.vid = cv2.VideoCapture(int(video_path))
        except:
            self.vid = cv2.VideoCapture(video_path)
        
        self.curentTracks = []
        self.lastTracks = []

    def tracking(self):
        return_value, frame = self.vid.read()
        if return_value:
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        else:
            return (return_value, None)

        frame_size = frame.shape[:2]
        image_data = cv2.resize(frame, (self.input_size, self.input_size))
        image_data = image_data / 255.
        image_data = image_data[np.newaxis, ...].astype(np.float32)
        start_time = time.time()
        
        # run detections
        self.infer = self.saved_model_loaded.signatures['serving_default']
        batch_data = tf.constant(image_data)
        pred_bbox = self.infer(batch_data)

        for key, value in pred_bbox.items():
            boxes = value[:, :, 0:4]
            pred_conf = value[:, :, 4:]

        boxes, scores, classes, valid_detections = tf.image.combined_non_max_suppression(
            boxes=tf.reshape(boxes, (tf.shape(boxes)[0], -1, 1, 4)),
            scores=tf.reshape(
                pred_conf, (tf.shape(pred_conf)[0], -1, tf.shape(pred_conf)[-1])),
            max_output_size_per_class=50,
            max_total_size=50,
            iou_threshold=self.iou,
            score_threshold=self.score
        )

        # convert data to numpy arrays and slice out unused elements
        num_objects = valid_detections.numpy()[0]
        bboxes = boxes.numpy()[0]
        bboxes = bboxes[0:int(num_objects)]
        scores = scores.numpy()[0]
        scores = scores[0:int(num_objects)]
        classes = classes.numpy()[0]
        classes = classes[0:int(num_objects)]

        # format bounding boxes from normalized ymin, xmin, ymax, xmax ---> xmin, ymin, width, height
        original_h, original_w, _ = frame.shape
        bboxes = utils.format_boxes(bboxes, original_h, original_w)

        # store all predictions in one parameter for simplicity when calling functions
        pred_bbox = [bboxes, scores, classes, num_objects]

        # read in all class names from config
        class_names = utils.read_class_names(cfg.YOLO.CLASSES)

        # by default allow all classes in .names file
        allowed_classes = list(class_names.values())
        
        # custom allowed classes (uncomment line below to customize tracker for only people)
        allowed_classes = ['car']

        # loop through objects and use class index to get class name, allow only classes in allowed_classes list
        names = []
        deleted_indx = []
        for i in range(num_objects):
            class_indx = int(classes[i])
            class_name = class_names[class_indx]
            if class_name not in allowed_classes:
                deleted_indx.append(i)
            else:
                names.append(class_name)
        names = np.array(names)

        # delete detections that are not in allowed_classes
        bboxes = np.delete(bboxes, deleted_indx, axis=0)
        scores = np.delete(scores, deleted_indx, axis=0)

        # encode yolo detections and feed to tracker
        features = self.encoder(frame, bboxes)
        detections = [Detection(bbox, score, class_name, feature) for bbox, score, class_name, feature in zip(bboxes, scores, names, features)]

        #initialize color map
        cmap = plt.get_cmap('tab20b')
        colors = [cmap(i)[:3] for i in np.linspace(0, 1, 20)]

        # run non-maxima supression
        boxs = np.array([d.tlwh for d in detections])
        scores = np.array([d.confidence for d in detections])
        classes = np.array([d.class_name for d in detections])
        indices = preprocessing.non_max_suppression(boxs, classes, self.nms_max_overlap, scores)
        detections = [detections[i] for i in indices]       

        # Call the tracker
        self.tracker.predict()
        self.tracker.update(detections)

        self.curentTracks = []
        # update tracks
        for track in self.tracker.tracks:
            if not track.is_confirmed() or track.time_since_update > 1:                
                #if not track.is_confirmed():
                    #print(track.get_class() + "-" + str(track.track_id) +' is missing')
                continue 
            bbox = track.to_tlbr()
            class_name = track.get_class()
            
        # draw bbox on screen
            color = colors[int(track.track_id) % len(colors)]
            color = [i * 255 for i in color]
            cv2.rectangle(frame, (int(bbox[0]), int(bbox[1])), (int(bbox[2]), int(bbox[3])), color, 2)
            cv2.rectangle(frame, (int(bbox[0]), int(bbox[1]-30)), (int(bbox[0])+(len(class_name)+len(str(track.track_id)))*17, int(bbox[1])), color, -1)
            cv2.putText(frame, class_name + "-" + str(track.track_id),(int(bbox[0]), int(bbox[1]-10)),0, 0.75, (255,255,255),2)
            self.curentTracks.append(class_name + "_" + str(track.track_id))

        cv2.putText(frame, 'count: '+ str(len(self.curentTracks)) + ' objects',(50, 150),0, 1, (255,0,0),3)
        idx = 0
        for lstName in self.lastTracks:
            if lstName not in self.curentTracks:
                idx+=1
        cv2.putText(frame, 'missing: '+ str(idx) + ' objects',(50, 100),0, 1, (255,0,0),3)        
        self.lastTracks = self.curentTracks

        # calculate frames per second of running detections
        fps = (int)(1.0 / (time.time() - start_time))
        cv2.putText(frame, "FPS: " + str(fps),(50, 50),0, 1, (255,0,0),3)

        result = np.asarray(frame)
        result = cv2.cvtColor(frame, cv2.COLOR_RGB2RGBA)
        return(return_value, result)