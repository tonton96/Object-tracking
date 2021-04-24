import tkinter
import PIL.Image, PIL.ImageTk
from tkinter import filedialog as fd
import my_object_tracker as obt

# a subclass of Canvas for dealing with resizing of windows
class ResizingCanvas(tkinter.Canvas):
    def __init__(self,parent,**kwargs):
        tkinter.Canvas.__init__(self,parent,**kwargs)
        self.bind("<Configure>", self.on_resize)
        self.height = self.winfo_reqheight()
        self.width = self.winfo_reqwidth()

    def on_resize(self,event):
        # determine the ratio of old width/height to new width/height
        wscale = float(event.width)/self.width
        hscale = float(event.height)/self.height
        self.width = event.width
        self.height = event.height
        # resize the canvas 
        self.config(width=self.width, height=self.height)
        # rescale all the objects tagged with the "all" tag
        self.scale("all",0,0,wscale,hscale)

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
        self.canvas = ResizingCanvas(self.root,width=850, height=400, bg="white", highlightthickness=0)
        self.canvas.pack(fill=tkinter.BOTH, expand=tkinter.YES)
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

if __name__ == '__main__':
    App() 