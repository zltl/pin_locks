#!/bin/python

import tkinter as tk
from tkinter import PhotoImage
import os.path
import os

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.grid()
        self.create_widgets()

    def create_widgets(self):
        self.cm_lb = tk.Label(self, text="命令")
        self.cm_lb.grid(row=0, column=0)
        self.hi_text = tk.Entry(self, width=40)
        self.hi_text.grid(row=0, column=1)

        self.hi_there = tk.Button(self)
        self.hi_there["text"] = "开始"
        self.hi_there["command"] = self.start_check
        self.hi_there.grid(row=0, column=2)

        #self.quit = tk.Button(self, text="退出",
        #                      command=root.destroy)
        #self.quit.grid(row=0, column=3)

    def start_check(self):
        print("start check")
        bug = False
        for i in range(0, 1000):
            bug = self.one_round(self.hi_text.get())
            if bug:
                break
        self.show_result(bug)
    def one_round(self, cmd):
        os.system("./rc test " + cmd + " >checker.log")
        if os.path.isfile("./lock_graph.dot"):
            return True
        return False

    def show_result(self, bug):
        t = tk.Toplevel(self)
        t.wm_title("结果")
        if (bug == False):
            l = tk.Label(t, text="没有发现错误")
            l.grid()
        else:
            os.system("dot lock_graph.dot -Tgif -o lock_graph.gif")
            photo = PhotoImage(file="lock_graph.gif")
            lb=tk.Label(t, image = photo)
            lb.grid(row=4, column=1)
            # l = tk.Label(t, text="xxxxx")
            l.grid()

if __name__ == "__main__":
    root = tk.Tk()
    root.wm_title("量天尺")
    app = Application(master=root)
    app.mainloop()
