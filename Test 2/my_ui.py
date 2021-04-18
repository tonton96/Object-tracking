from tkinter import*
from tkinter import filedialog as fd
import my_object_tracker as obt

def openFile():
    name = fd.askopenfilename(initialdir = "/",title = "Select file",filetypes = (("Video file","*.mp4"),("Video file","*.*")))
    tracker = obt.MyTracker(name)
    tracker.tracking()

root = Tk()
root.title("Object tracking")
root.geometry("400x200")
openFileBtn = Button(root, text = "Open file",width =100, height=100, command=openFile)
openFileBtn.pack()
root.mainloop()