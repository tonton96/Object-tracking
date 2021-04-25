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
            self.openFileBtn['state'] = 'disable'
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
                im = PIL.Image.fromarray(frame)
                imgtk = PIL.ImageTk.PhotoImage(image=im)
                wide = imgtk.width()
                high = imgtk.height()
                viewwide = 1280
                viewhigh = 720
                if wide > viewwide or high > viewhigh:
                    wide_factor = viewwide / wide
                    high_factor = viewhigh / high
                    factor = min(wide_factor, high_factor)
                    wide = int(wide * factor)
                    if wide <= 0 : wide = 1
                    high = int(high * factor)
                    if high <= 0 : high = 1
                    im=im.resize((wide, high), PIL.Image.ANTIALIAS)
                    imgtk = PIL.ImageTk.PhotoImage(image=im)

                self.photo = imgtk                
                self.canvas.create_image(0, 0, image = self.photo, anchor = tkinter.NW)
                self.root.after(self.delay, self.showVideo)
            else:
                self.openFileBtn['state'] = 'normal'

if __name__ == '__main__':
    App() 