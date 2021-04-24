import tkinter
import PIL.Image, PIL.ImageTk
from tkinter import filedialog as fd
import my_object_tracker as obt

class App:
    def openFile(self):
        name = fd.askopenfilename(initialdir = "/",title = "Select file",filetypes = (("Video file","*.mp4"),("Video file","*.*")))
        if name:
            self.tracker = obt.MyTracker(name)
            self.tracker.trackingInit()
            self.showVideo()
        else:
            self.tracker = None

    def __init__(self):
        self.root = tkinter.Tk()
        self.root.title("Object tracking")
        self.root.geometry("1280x720")
        self.canvas = tkinter.Canvas(self.root, width = 1280, height = 720)
        self.canvas.pack()
        self.tracker = None
        self.delay = 1

        self.openFileBtn = tkinter.Button(self.root, text = "Open file", command=self.openFile)
        self.openFileBtn.pack()
        self.root.mainloop()
    
    def showVideo(self):
        if self.tracker:
            (ret, frame) = self.tracker.tracking() 
            if ret:
                self.photo = PIL.ImageTk.PhotoImage(image = PIL.Image.fromarray(frame))
                self.canvas.create_image(0, 0, image = self.photo, anchor = tkinter.NW)
                self.root.after(self.delay, self.showVideo)       

app = App()