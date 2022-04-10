# plot.py
import os
from pathlib import Path
import matplotlib.pyplot as plt
import matplotlib.patches as patches

#fig = plt.figure()

#fig, ax = plt.subplots(figsize)
#ax.plot([,100],[1,100])

def plot_rectangle(ax, x1, y1, x2, y2):
    print('draw: ( ' + str(x1) + ' , ' + str(y1) + ') , w =' + str(x2 - x1) + ' , h = ' + str(y2 - y1))
    ax.add_patch(
        patches.Rectangle(
            (x1, y1),
            (x2 - x1),
            (y2 - y1),
            fill = True,        
        )
    )


def read_rpt(filename):
    file = open(filename, 'r')
    testcase = Path(filename).stem

    fig, ax = plt.subplots()
    ax.set_title(testcase)
    ax.set_xlabel('x-axis')
    ax.set_ylabel('y-axis')
    ax.grid(which = 'major', axis = 'both')
    plt.rc('font', size = 6)

    ax.plot([0], [0])

    # <final cost> 
    file.readline()

    # <total wirelength>
    file.readline()

    # <chip_area>
    file.readline()

    # <chip_width> <chip_height>
    chip_width, chip_height = file.readline().split()
    chip_width = int(chip_width)
    chip_height = int(chip_height)
    ax.plot(chip_width, chip_height, 'g*')
    plt.text(chip_width, chip_height, '({}, {})'.format(chip_width, chip_height))  

    # <program_runtime>    
    file.readline()

    # <macro_name> <x1> <y1> <x2> <y2> 
    x_list = []
    y_list = []
    for line in list(file):
        name, x1, y1, x2, y2 = line.split()
#        print('name : ' + name + '( ' + x1 + ' , ' + y1 + ') , ( ' + x2 + ' , ' + y2 + ' )')
        x1 = int(x1)
        y1 = int(y1)
        x2 = int(x2)
        y2 = int(y2)
        x_list.extend([x1]) #, x2])
        y_list.extend([y1]) #, y2])
        w = x2 - x1
        h = y2 - y1
        cx = x1 + w / 2.0
        cy = y1 + h / 2.0
        rect = patches.Rectangle((x1, y1), w, h, edgecolor = 'black', fill = True)
        tag = name + '\n' + str(w) + ', ' + str(h)
        ax.annotate(tag, (cx, cy), color = 'white', ha = 'center', va = 'center')
        ax.add_patch(rect)
    
    ax.plot(x_list, y_list, 'r*')
    for x, y in zip(x_list, y_list):
        plt.text(x, y, '({}, {})'.format(x, y))        

    plot_name = './plot/' + testcase + '.png'
    plt.savefig(plot_name)
    print('save ' + plot_name)

if __name__ == '__main__':
    directory = 'output_pa2'
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if filename[0] == '.':
            continue
        if filename.find(".HPWL") != -1:
            continue
        if os.path.isfile(f):
            print("now: " + f)
            read_rpt(f)
