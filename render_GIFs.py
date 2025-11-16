
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
def render(gridsize, object_data, barrier_data=None, food_data=None, circleDiameter=30, spacing=0, deathArea=None):
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
        top_left = (barrier[1] * cellSize + spacing // 2 + 0.5, barrier[0] * cellSize + spacing // 2 + 0.5) 
        bottom_right = (top_left[0] + circleDiameter, top_left[1] + circleDiameter)
        draw.rectangle([top_left, bottom_right], fill=(93, 85, 85)) # grey: 0x5d5555
    
    for food in food_data:
        top_left = (food[1] * cellSize + spacing // 2 + 0.5, food[0] * cellSize + spacing // 2 + 0.5) 
        bottom_right = (top_left[0] + circleDiameter, top_left[1] + circleDiameter)
        draw.polygon(((top_left[0] + cellSize/2, top_left[1] + cellSize/10), (bottom_right[0],bottom_right[1]), (bottom_right[0] - cellSize, bottom_right[1])), fill=(255, 255, 0), outline="black")                
            #sticker = Image.open("flower30x30.jpg")
            #image.paste(sticker, (top_left[0], top_left[1]))
       
        
    gif_frames.append(image)

def render_pixels(gridsize, object_data, barrier_data, food_data, cellWidth=1, deathArea=None):

    imgSizeX = cellWidth * gridsize[0]
    imgSizeY = cellWidth * gridsize[1]

    # draw data on a numpy array for very fast pixel operations
    grid = np.zeros((imgSizeY, imgSizeX, 3), dtype=np.uint8)
    grid.fill(255) # white

    # draw death areas manually for different keys
    if deathArea == 0:
        for x in range(0, imgSizeX//2):
            for y in range(0, imgSizeY):
                grid[x, y] = (255, 70, 70)

    # draw object data
    for object_ in object_data:
        # get color
        rgb_color = (object_[3], object_[4], object_[5])

        # get position (y, x)
        top_left = (object_[1]*cellWidth, object_[0]*cellWidth) # (x, y)
        bottom_right = (top_left[0] + cellWidth, top_left[1] + cellWidth) # (x+dx, y+dy)

        for x in range(top_left[0], bottom_right[0]):
            for y in range(top_left[1], bottom_right[1]):
                grid[y, x] = rgb_color

    # draw barriers
    for object_ in barrier_data:
        # get position (y, x)
        top_left = (object_[1]*cellWidth, object_[0]*cellWidth) # (x, y)
        bottom_right = (top_left[0] + cellWidth, top_left[1] + cellWidth) # (x+dx, y+dy)

        for x in range(top_left[0], bottom_right[0]):
            for y in range(top_left[1], bottom_right[1]):
                grid[y, x] = (93, 85, 85)

    # draw food
    for object_ in food_data:
        # get position (y, x)
        top_left = (object_[1]*cellWidth, object_[0]*cellWidth) # (x, y)
        bottom_right = (top_left[0] + cellWidth, top_left[1] + cellWidth) # (x+dx, y+dy)

        for x in range(top_left[0], bottom_right[0]):
            for y in range(top_left[1], bottom_right[1]):
                grid[y, x] = (255, 255, 0)

    image = Image.fromarray(grid, "RGB")
    gif_frames.append(image)
    

def create_gif(filename="sandbox.gif", frameduration=150):   
    "save in cwd"
    gif_frames[0].save(filename, save_all=True, append_images=gif_frames[1:],duration=frameduration, loop=0)
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

            # first line tells us world size y, x, render_resolution, frame duration, selection criterium
            firstline = textfile.readline()
            header = firstline.strip("\n").split(",")
            ysize = header[0]
            xsize = header[1]
            resolution = header[2]
            frameduration = header[3]
            selCrit = header[4]

            # second line tells us the position of barriers
            secondline = textfile.readline()
            barrier_data = []
            if len(secondline.strip("\n")) > 0:
                barrier_positions = secondline.strip("\n").split(",") # positions in format y;x
                barrier_data = [tuple(map(int, i.split(";"))) for i in barrier_positions]

            allLines = textfile.readlines()

            #two lines make up one simtep, so range in steps of 2
            for i in range(0, len(allLines), 2):
                if i > 1:
                    line1 = allLines[i]

                    object_data = []
                    eachObj = line1.split(",")

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

                    line2 = allLines[i+1]
                    food_data = []
                    if len(line2.strip("\n")) > 0:
                        interactives_data = line2.split(",")
                        for int_ in interactives_data:
                            elem = int_.split(";")

                            # int PosY, int PosX, int type
                            data = (elem[2], (int(elem[0]), int(elem[1])))
                            if data[0] == "-3": # -3 = FOOD in World.h
                                food_data.append(data[1])
                    # render
                    if int(resolution) < 4:
                        render_pixels((int(xsize), int(ysize)), object_data, barrier_data, food_data, cellWidth=int(resolution), deathArea=death_area)
                    else:
                        render((int(xsize), int(ysize)), object_data, barrier_data, food_data, circleDiameter=int(resolution), deathArea=death_area)


            # create GIF for generation
            fileName = "generation_" + generation_num + ".gif"
            create_gif(filename=fileName, frameduration=int(frameduration))
            clear_gif()

renderGenerations()