
from PIL import Image, ImageDraw
import numpy as np
import gc
import os
import math

from pyparsing import Each

# set directory
with open("folderdir.txt") as file:
    folder_dir = file.readline().strip("\n")
    os.chdir(folder_dir)

gif_frames = []

# render dis shit
def render(object_data, barrier_data, food_data, gridsize, circleDiameter=30, spacing=0, deathArea=None):
    # object_data:
    # (shape; yxPos[0]; yxPos[1]; facing; color[0]; color[1]; color[2])
    #    0       1          2       3         4        5         6

    cellSize = circleDiameter + spacing
    imgSizeX = cellSize * gridsize[0]
    imgSizeY = cellSize * gridsize[1]

    # create a blank canvas
    image = Image.new("RGB", (imgSizeX, imgSizeY), color="white")
    draw = ImageDraw.Draw(image)

    # if a death area should be drawn, do it now
    if deathArea == 0:
        draw.rectangle([ (imgSizeX//2, 0), (imgSizeX, imgSizeY) ], fill=(255, 70, 70))

    # for each object (pixie), draw a circle
    for object_ in object_data:
        # get color
        rgb_color = (object_[3], object_[4], object_[5])

        # Berechne die Position des Kreises
        top_left = (object_[1] * cellSize + spacing // 2 + 0.5, object_[0] * cellSize + spacing // 2 + 0.5) 
        bottom_right = (top_left[0] + circleDiameter, top_left[1] + circleDiameter) 

        
        draw.ellipse([top_left, bottom_right], fill=rgb_color)

        facing_y = math.sin(object_[2]) * (circleDiameter/2) + 0.5
        facing_x = math.cos(object_[2]) * (circleDiameter/2) + 0.5
        x_point = object_[1] * cellSize + spacing + circleDiameter/2 + facing_x
        y_point = object_[0] * cellSize + spacing + circleDiameter/2 + facing_y

        draw.circle(xy=(x_point, y_point), radius=circleDiameter/7, fill=rgb_color)
    
    for barrier in barrier_data:
        draw.rectangle([top_left, bottom_right], fill=rgb_color)
    
    for food in food_data:
        draw.polygon(((top_left[0] + cellSize/2, top_left[1] + cellSize/10), (bottom_right[0],bottom_right[1]), (bottom_right[0] - cellSize, bottom_right[1])), fill=rgb_color, outline="black")                
            #sticker = Image.open("flower30x30.jpg")
            #image.paste(sticker, (top_left[0], top_left[1]))
       
        
    gif_frames.append(image)
    

def create_gif(filename="sandbox.gif"):   
    "save in cwd"
    gif_frames[0].save(filename, save_all=True, append_images=gif_frames[1:],duration=150, loop=0)
    clear_gif()

def clear_gif():
    "clears the gif_frames list"
    gif_frames.clear()
    gc.collect


# extract all pixie information for each simstep, then render a single frame
def renderGenerations():
    folder = folder_dir + "/GIF data"

    print("rendering...")

    for filename in os.listdir(folder):
        # whole path
        filepath = os.path.join(folder, filename)

        # extract generation number
        filenamewithending = filename.split(".")
        GIFdata_X = filenamewithending[0].split("_")
        generation_num = GIFdata_X[1] # still str

        # open file and start extracting object data
        with open(filepath) as textfile:

            # first line tells us world size y, x, render_resolution, selection criterium
            firstline = textfile.readline()
            header = firstline.strip("\n").split(",")
            ysize = header[0]
            xsize = header[1]
            resolution = header[2]
            selCrit = header[3]

            allLines = textfile.readlines()

            #each line is one simStep = frame  #NOT FOR LONG: 3 lines = 1 simstep
            for i, line in enumerate(allLines):
                if i > 0:
                    object_data = []
                    eachObj = line.split(",")

                    for obj in eachObj:                   
                        elem = obj.split(";")
                            
                        # int PosY, int PosX, float facing, int R, int G, int B
                        data = (int(elem[0]), int(elem[1]), float(elem[2]), 
                                int(elem[3]), int(elem[4]), int(elem[5]))
                        object_data.append(data)
                    
                    if i > len(allLines)-6: # render a death area for the last 5 frames
                        death_area = selCrit
                    else:
                        death_area = None

                    barrier_data = [] #for now
                    food_data = []

                    render(object_data, barrier_data, food_data, (int(xsize), int(ysize)), circleDiameter=int(resolution), deathArea=death_area)


            # create GIF for generation
            fileName = "generation_" + generation_num + ".gif"
            create_gif(filename=fileName)
            clear_gif()

renderGenerations()