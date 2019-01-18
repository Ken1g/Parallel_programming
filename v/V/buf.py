from kivy.app import App
from kivy.clock import Clock
from kivy.graphics import Fbo, Rectangle
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.widget import Widget
from array import array

size_of_cell = 10
ISIZE = 100
JSIZE = 100



class FboTest(Widget):
    def __init__(self, **kwargs):
        super(FboTest, self).__init__(**kwargs)
        self.fbo_size = size_of_cell
        self.fbo = Fbo(size=(self.fbo_size * ISIZE, self.fbo_size * JSIZE))
        self.koef = 255.0 / 400.0  

        my_file = open("YA")

        self.file = my_file
        with self.canvas:
            Rectangle(size=(self.fbo_size * ISIZE, self.fbo_size * JSIZE), texture=self.fbo.texture)

        size = self.fbo_size * self.fbo_size * ISIZE * JSIZE
        buf = [20, 0, 0] * size  # initialize buffer to all red pixels

        self.arr = array('B', buf)

        my_string = []
        for i in range(ISIZE):
            my_string.append([float(i) for i in self.file.readline().split()])
        print(my_string)
        for idx, i in enumerate(my_string):
            for jdx, i in enumerate(i):
                temperature = round(my_string[idx][jdx] * self.koef)
                self.draw_cell(idx, jdx, temperature)
        self.file.readline()

        self.fbo.texture.blit_buffer(self.arr, colorfmt='rgb', bufferfmt='ubyte')

    def draw_table(self, *args):

        Clock.schedule_interval(self.draw_iter, 0.5)
        #    my_string = []
        #    for i in range(ISIZE):
        #        my_string.append([float(i) for i in my_file.readline().split()])
        #    for idx, i in enumerate(my_string):
        #        for jdx, i in enumerate(i):
        #            temperature = round(my_string[idx][jdx] * koef)
        #            self.draw_cell(idx, jdx, temperature)
        #    self.fbotest.fbo.texture.blit_buffer(self.fbotest.arr, colorfmt='rgb', bufferfmt='ubyte')
        #    my_file.readline()

        #my_file.close()

    def draw_iter(self, dt):
        my_string = []
        for i in range(ISIZE):
            my_string.append([float(i) for i in self.file.readline().split()])
        print(my_string)
        for idx, i in enumerate(my_string):
            for jdx, i in enumerate(i):
                temperature = round(my_string[idx][jdx] * self.koef)
                self.draw_cell(idx, jdx, temperature)
        print('did it')
        self.fbo.texture.blit_buffer(self.arr, colorfmt='rgb', bufferfmt='ubyte')
        self.file.readline()
    
    def draw_cell(self, i, j, temperature):
        
        for x in range(size_of_cell * i, size_of_cell * (i + 1)):
            for y in range(size_of_cell * j, size_of_cell * (j + 1)):
                self.set_pixel(x, y, temperature)
        with self.canvas:
            Rectangle(size=(0,0))
            

    def set_pixel(self, x, y, temperature):
        index = y * ISIZE * self.fbo_size * 3 + x * 3
        self.arr[index] = 0
        self.arr[index+1] = 0
        self.arr[index+2] = temperature 

class FboPlayApp(App):

    def build(self):
        root = FloatLayout(size_hint=(None, None), size=(750, 750))
        self.fbotest = FboTest(size_hint=(None, None), size=(512, 512))
        button = Button(
            text='Start simulation', 
            size_hint=(None, None), 
            size=(200, 500), 
            pos=(1100, 200), 
            on_release=self.fbotest.draw_table
        )
        button1 = Button(
            text='FUCK', 
            size_hint=(None, None), 
            size=(200, 200), 
            pos=(1400, 100), 
            on_release=self.fbotest.draw_table
        )
        root.add_widget(self.fbotest)
        root.add_widget(button)
        root.add_widget(button1)

        return root
if __name__ == "__main__":
    FboPlayApp().run()
