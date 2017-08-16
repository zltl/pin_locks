#!/bin/python

import tkinter as tk

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
        print("hi there, everyone!")
        self.show_result()

    def show_result(self):
        t = tk.Toplevel(self)
        t.wm_title("结果")
        l = tk.Label(t, text="This is result")
        l.grid()

if __name__ == "__main__":
    root = tk.Tk()
    root.wm_title("量天尺")
    app = Application(master=root)
    app.mainloop()
