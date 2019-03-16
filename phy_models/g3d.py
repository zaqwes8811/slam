#!/usr/bin/env python
# coding: utf-8

import sys
sys.path.append("/home/zaqwes")

#import code.book_plots as book_plots
#import book_format
#import code.gh_internal as gh
#from code.gh_internal import plot_gh_results

import numpy as np
from numpy.linalg import inv
from numpy.random import randn

import matplotlib.pyplot as plt
from matplotlib.pyplot import show, grid, plot, xlim, ylim
import matplotlib.cm as cm

import cv2

from matlab_ext.plotter import implot, implot_ax, close_all

    
#
#
#
def get_matrix( src, dst ):
    #http://math.stackexchange.com/questions/296794/finding-the-transform-matrix-from-4-projected-points-with-javascript   
    return np.matrix( cv2.getPerspectiveTransform(src, dst) )
     
def toImg(x, y, m):
    #warped = cv2.warpPerspective(img, M, (maxWidth, maxHeight))
    xy = np.matrix( [x, y, 1], dtype="float" )
    img_xy = m*xy.T
    img_xy /= img_xy[2]
    a = np.array( img_xy.squeeze(), dtype="float" )
    a = tuple( a[0][0:2] )
    return a
    
def fromImg( x, y, m ):
    xy = np.matrix( [x, y, 1], dtype="float" ).T
    a = inv(m)*xy
    a /= a[2]
    a = np.array( a.squeeze(), dtype="float" )
    a = tuple( a[0][0:2] )
    return a[0], a[1]
    
def src_dst():
    w, h = 200, 300
    x, y = 150, 10
    src = np.array([[x, y], [x+w, y], 
                    [x+w, y+h], [x, y+h]], dtype="float32") 
    dst = np.array([[x+20, y+100], [x+w-20, y+100], 
                    [x+w+50, y+h-50], [x-50, y+h-50]], dtype = "float32")
    return src, dst
#
#
#
def main():    
    close_all()
    f, (ax_img, ax_phy) = plt.subplots(1, 2)
    ax_img.grid()
    ax_phy.grid()
    
    # Генерируем исходне изображение
    shape = 768/2, 1024/2
    img = 100*np.ones( shape, np.uint8 )
       
    # Получаем матрицу гомографического преобразования
    src, dst = src_dst()
    for p in src:    
        implot_ax( ax_phy, p[0], p[1], '-r+' )   
    for p in dst:    
        implot_ax( ax_img, p[0], p[1], '-g+' )
        
    m = get_matrix( src, dst )
   
    # Искривляем метрицей прямоугольник
    phxs, phys = [], []
    xs, ys = [], []
    for i in range( 50 ):
        r_x = 150
        x,y = (r_x, 10+i*5)
        ix, iy = toImg( x,y, m )
        ix += randn() * 4        
        iy += randn() * 2   
        
        x, y = fromImg( ix, iy, m )
        
        xs.append( ix )
        ys.append( iy )
        phxs.append( r_x )
        phys.append( y )      
    
    implot_ax( ax_phy, phxs, phys, '-rv' )       
    implot_ax( ax_img, xs, ys, '-g' )
    
    # Рисуем что получили в виде полиготов
    ax_phy.set_xlim([0, shape[1]])
    ax_phy.set_ylim([-shape[0], 0])
    ax_img.set_xlim([0, shape[1]])
    ax_img.set_ylim([-shape[0], 0])
    show()
    
if __name__ == '__main__':
    main()

