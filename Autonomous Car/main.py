# -*- coding: utf-8 -*
import sys
import math
import cv2
import numpy as np
import time
import threading
import zodo
import lider
t2 = None
from bottle import route,run,get,post,response,static_file,request
from customControlServer import *
import customControlServer
#control rccar
@post('/motor')
def control_rccar():
    command=request.forms.get('command')
    print (command)
    if command == "AUTO": auto()
    if customControlServer.OnOff == False:
        if command == "GO": forward()
        elif command == "LEFT": left()
        elif command == "STOP": stop()
        elif command == "RIGHT": right()
        elif command == "BACK": backward()
        elif command == "UP": speedUp()
        elif command == "DOWN": speedDown()
        elif command == "SLOWLEFT": slowLeft()
        elif command == "SLOWRIGHT": slowRight()
    return ''

@route('/')
def do_route():
    return static_file("index.html", root=".")



COLUMN = 640
ROW = 360
direction = None
flag = 0
OnOff = 1
class motorAction:
    def __init__(self,target,index):
        self.state = target
        if index > 180:
            self.reverse = 1
        else:
            self.reverse = -1
    def whereControl(self):
        where = 320-self.state
        lev = int(where) / 30     #130까지 나왔음 130%30은 4 그 이상은 5단계로하자
        #print(lev)
        level = self.reverse * round(lev)
        print('xt =' + str(self.state) + ' lev = ' + str(level)+'yt = ' + str(yt) + 'self.reverse = ' + str(self.reverse))
        return level
 

def region_of_interest(img, vertices):  # ROI 셋팅
    mask = np.zeros_like(img)  # mask = img와 같은 크기의 빈 이미지
    if len(img.shape) > 2:  # Color 이미지(3채널)라면 :
        channel_count=img.shape[2]
        ignor_mask_color=(255,)*channel_count
    else:  # 흑백 이미지(1채널)라면 :
        ignor_mask_color=255
    # vertices에 정한 점들로 이뤄진 다각형부분(ROI 설정부분)을 color로 채움
    cv2.fillPoly(mask, vertices, ignor_mask_color)
    # 이미지와 color로 채워진 ROI를 합침
    ROI_image = cv2.bitwise_and(img, mask)
    return ROI_image

cap = cv2.VideoCapture(0) #연결된 웹캠으로 비디오 연결(0)
class def0(threading.Thread):
    def run(self):
        zodo.OnOffLED()
        
class def1(threading.Thread):
    def run(self):
        try:
            lider.getliderData()
        except KeyboardInterrupt: # Ctrl+C
            if lider.ser != None:
                lider.ser.close()
                
class def2(threading.Thread):
    global flag,direction,yt
    point = 0
    def run(self):
        while 1:
            print(direction)
            initMotors()
            run(host='192.168.43.28', port=8080)
            time.sleep(0.1)
            

zodoModule = def0(name = "0")
liderModule = def1(name = "1")
motorModule = def2(name = "2")
zodoModule.start()
liderModule.start()
motorModule.start()
previous = 0

t1 = time.time()
while (True):
    if t2 == None:
        t2 = time.time()
    ret, src = cap.read() #src는 프레임임(원본이미지)
    src=cv2.resize(src,(640,360)) #가로640 세로 360
    if customControlServer.OnOff == True:
        if t2 - t1 >= 0.2:
            src_clone = cv2.GaussianBlur(src, (5,5), 0)
            cdst = cv2.Canny(src_clone, 30, 200, None, 3) # dst를 캐니 여기서 괄호는 각각(사진, 임계값1,임계값2) 이다 임계값 1이하면 가장자리 제외
            #cdst = cv.cvtColor(dst, cv.COLOR_GRAY2BGR) #캐니엣지 처리된 dst를 그레이 스케일로
            cdstP = np.copy(cdst)#cdst의 똑같은 크기의 사진(배열)을 만듦

            #### 관심영역(ROI) 지정하는 부분
            imshape = src.shape
            vertices = np.array([[(0,0),
                                  (640, 0),
                                  (640, 180),
                                  (0, 180)]])
            mask = region_of_interest(cdst,vertices)#edge중 관심영역만 가져옴
            ####
            #                   (검출이미지, 거리, 각도, 임곗값) 임곗값은 누산평면의값(누산평면은 각도x거리)
            lines=cv2.HoughLines(mask,1,np.pi/180,40)# 관심영역에서 선을그림
            lines_img=np.zeros((src.shape[0],src.shape[1],3),dtype=np.uint8)
            #cv2.imshow('lines_img', lines_img)     #그냥 흑색화면
            listxp1 =[]
            listxp2 =[]
            listyp1 = []
            listyp2 = []
            listxm1 =[]
            listxm2 =[]
            listym1 = []
            listym2 = []
            #print(len(lines))           #일순간의 허프변환을 통했을때 그 순간의 선의 길이?
            if lines is not None:
                for i in range(0, len(lines)):
                    # x1,y1,x2,y2 -> 직선의 좌표구하는것(x1,y1) ~ (x2,y2)
                    r, theta = lines[i][0]  #허피 변환을 하면 직선이 나올수 있는 모든 경우의 수의 기울기를 뽑아줌
                    a = np.cos(theta)
                    b = np.sin(theta)
                    x0 = a* r
                    y0 = b* r
                    x1 = int(x0 + 1000*(-b))
                    y1 = int(y0 + 1000*(a))
                    x2 = int(x0 - 1000*(-b))
                    y2 = int(y0 - 1000*(a))
                    # 간혹 직선의 경우의수가 안뽑힐 때도있음, 실제 테스트 해보고 캐니엣지를 건드려줘야함 이럴때

                    try:
                        cv2.line(lines_img,(x1,y1),(x2,y2),(0,255,0),2)
                        if y1 > 0:
                            slope = abs(y2 - y1) / abs(x1 - x2)
                        else:
                            slope = abs(y1 - y2) / abs(x1 - x2)
                        if 0.2 < slope < 30:  # 1차적으로 쓸모없고 평균값 잡아먹는 말도안되는 기울기를 가진 직선을 1차적으로 필터링해줌
                        #if (y0 > 100 and x0 > 100) or (y0 < 100 and x0 < 100):
                            if y1 > 0:  # 이안에 들어있으면 우선 Y1과 Y2를 판별부터함 이게 섞여있어서 Y1이 양수일 때도 있고 음수일때도있음
                                # x 축은 무조건 x1  = -값 x2는 무조건 +값이라서 y1이 양수면 2사분면에서 4사분면, y1이 음수면 3사분면에서 1사분면으로 이동
                                # 이거를 따로따로 리스트에 전부 구해서 추가시켜줄거임
                                listxm1.append(x2)
                                listxm2.append(x1)
                                listym1.append(y2)
                                listym2.append(y1)
                                cv2.line(lines_img,(x1,y1),(x2,y2),(0,0,255),2)
                            else:
                                listxp1.append(x1)
                                listxp2.append(x2)
                                listyp1.append(y1)
                                listyp2.append(y2)
                                cv2.line(lines_img,(x1,y1),(x2,y2),(255,0,0),2)
                            #print(f' i = {i} x1 = {x1}, x2 = {x2} , y1 = {y1}, y2 = {y2} , y0 = {y0}, x0 = {x0}')
                            #print(f' i = {i} x1 = {x1}, x2 = {x2} , y1 = {y1}, y2 = {y2}')  여기
                        

                    except ZeroDivisionError:   #직선이 되는 경우의수가 없는경우 그냥 건너뜀 어차피 while문안이고, 개빠르게 계산하기 때문에 금방 넘어감
                        t1 = time.time() - 0.2
                        t2 = time.time()
                        listxp1 = []
                        listxp2 = []
                        listyp1 = []
                        listyp2 = []
                        listxm1 = []
                        listxm2 = []
                        listym1 = []
                        listym2 = []
                        continue
                try:
                    px1 = int(np.mean(listxp1))  #리스트에잇는것들 다 평균값 구함     #p로시작하는게 왼쪽직선  밑에m으로 시작하는게 오른쪽직선
                    px2 = int(np.mean(listxp2))
                    py1 = int(np.mean(listyp1))
                    py2 = int(np.mean(listyp2))
                    cv2.line(mask, (px1, py1), (px2, py2), (255, 255, 255), 2)
                    #cv2.line(src, (px1, py1), (px2, py2), (255, 255, 255), 2)
                    mx1 = int(np.mean(listxm1))
                    mx2 = int(np.mean(listxm2))
                    my1 = int(np.mean(listym1))
                    my2 = int(np.mean(listym2))
                    cv2.line(mask, (mx1, my1), (mx2, my2), (255, 255, 255), 2)
                    #cv2.line(src, (mx1, my1), (mx2, my2), (255, 255, 255), 2)
                    Pa = (py2 - py1) / (px2 - px1)  #직선의 방정식을 이용하여 소실점 구하는 공식       좌측 직선 기울기
                    Ma = (my2 - my1) / (mx2 - mx1)  #직선의 방정식을 이용하여 소실점 구하는 공식       우측 직선 기울기

                    #print(int(Pa*100))      #44,-85일때 직진중
                    #print(int(Ma*100))
                    xt = int(((Pa * px1)-py1 - (Ma*mx1) + my1) / (Pa - Ma))  #직선의 방정식을 이용하여 소실점 구하는 공식      #이 xt yt가 소실점
                    yt = int((Pa * (xt - px1)) + py1)  #직선의 방정식을 이용하여 소실점 구하는 공식
                    #print(xt)
                    #print(yt)
                    cv2.line(src, (320, 0), (320, 360), (0, 0, 255), 4)     #중앙선(이거는 우리가 설정하는 값이므로 상수형태로 그냥 썼다)
                    car = motorAction(xt,yt)
                    direction = car.whereControl()
                    #if previous != direction:
                        #flag = 1
                    #else:
                        #flag = 0
                    #previous = direction
                    #print("direction = %s, flag = %s" %(direction,flag))
                    cv2.line(src,(xt,yt),(xt,yt),(255,0,0),9)         #소실점  영상이 한쪽에 치우쳐져있어서 임시로 40을 저한거임
                except ValueError:   #직선이되는 경우의 수가 없어 zerodivision 오류가떠서 pass가 났다. 그럼 리스트에 아무 값도 들어있지 않아서 이것도 말이안됨 따라서, 이부분도 패스시켜줌
                    pass
            t1 = time.time()
            t2 = time.time()
            cv2.imshow("ROI",lines_img)
            #cv2.imshow('cdstP',cdstP)
            cv2.imshow('source',mask)
            previous = 1
        else:
            t2 = time.time()
    if previous == 1 and customControlServer.OnOff == False:
        cv2.destroyWindow("ROI")
        cv2.destroyWindow('source')
        previous = 0
    cv2.imshow('video', src)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
"""
# -*- coding: utf-8 -*
import sys
import math
import cv2
import numpy as np
import time
import threading
import zodo
import lider
t2 = None
from bottle import route,run,get,post,response,static_file,request
from customControlServer import *
import customControlServer
#control rccar
@post('/motor')
def control_rccar():
    command=request.forms.get('command')
    print (command)
    if command == "AUTO": auto()
    if customControlServer.OnOff == False:
        if command == "GO": forward()
        elif command == "LEFT": left()
        elif command == "STOP": stop()
        elif command == "RIGHT": right()
        elif command == "BACK": backward()
        elif command == "UP": speedUp()
        elif command == "DOWN": speedDown()
        elif command == "SLOWLEFT": slowLeft()
        elif command == "SLOWRIGHT": slowRight()
    return ''

@route('/')
def do_route():
    return static_file("index.html", root=".")



COLUMN = 640
ROW = 360
direction = None
flag = 0
OnOff = 1
class motorAction:
    def __init__(self,target,index):
        self.state = target
        if index > 180:
            self.reverse = 1
        else:
            self.reverse = -1
    def whereControl(self):
        where = 320-self.state
        lev = int(where) / 30     #130까지 나왔음 130%30은 4 그 이상은 5단계로하자
        #print(lev)
        level = self.reverse * round(lev)
        print('xt =' + str(self.state) + ' lev = ' + str(level)+'yt = ' + str(yt) + 'self.reverse = ' + str(self.reverse))
        return level
 

def region_of_interest(img, vertices):  # ROI 셋팅
    mask = np.zeros_like(img)  # mask = img와 같은 크기의 빈 이미지
    if len(img.shape) > 2:  # Color 이미지(3채널)라면 :
        channel_count=img.shape[2]
        ignor_mask_color=(255,)*channel_count
    else:  # 흑백 이미지(1채널)라면 :
        ignor_mask_color=255
    # vertices에 정한 점들로 이뤄진 다각형부분(ROI 설정부분)을 color로 채움
    cv2.fillPoly(mask, vertices, ignor_mask_color)
    # 이미지와 color로 채워진 ROI를 합침
    ROI_image = cv2.bitwise_and(img, mask)
    return ROI_image

cap = cv2.VideoCapture(0) #연결된 웹캠으로 비디오 연결(0)
class def0(threading.Thread):
    def run(self):
        zodo.OnOffLED()
        
class def1(threading.Thread):
    def run(self):
        try:
            lider.getliderData()
        except KeyboardInterrupt: # Ctrl+C
            if lider.ser != None:
                lider.ser.close()
                
class def2(threading.Thread):
    global flag,direction,yt
    point = 0
    def run(self):
        while 1:
            print(direction)
            initMotors()
            run(host='192.168.43.28', port=8080)
            time.sleep(0.1)
            

zodoModule = def0(name = "0")
liderModule = def1(name = "1")
motorModule = def2(name = "2")
zodoModule.start()
liderModule.start()
motorModule.start()
previous = 0

t1 = time.time()
while (True):
    if t2 == None:
        t2 = time.time()
    ret, src = cap.read() #src는 프레임임(원본이미지)
    src=cv2.resize(src,(640,360)) #가로640 세로 360
    if customControlServer.OnOff == True:
        if t2 - t1 >= 0.2:
            src_clone = cv2.GaussianBlur(src, (5,5), 0)
            cdst = cv2.Canny(src_clone, 30, 200, None, 3) # dst를 캐니 여기서 괄호는 각각(사진, 임계값1,임계값2) 이다 임계값 1이하면 가장자리 제외
            #cdst = cv.cvtColor(dst, cv.COLOR_GRAY2BGR) #캐니엣지 처리된 dst를 그레이 스케일로
            cdstP = np.copy(cdst)#cdst의 똑같은 크기의 사진(배열)을 만듦

            #### 관심영역(ROI) 지정하는 부분
            imshape = src.shape
            vertices = np.array([[(0,0),
                                  (640, 0),
                                  (640, 180),
                                  (0, 180)]])
            mask = region_of_interest(cdst,vertices)#edge중 관심영역만 가져옴
            ####
            #                   (검출이미지, 거리, 각도, 임곗값) 임곗값은 누산평면의값(누산평면은 각도x거리)
            lines=cv2.HoughLines(mask,1,np.pi/180,40)# 관심영역에서 선을그림
            lines_img=np.zeros((src.shape[0],src.shape[1],3),dtype=np.uint8)
            #cv2.imshow('lines_img', lines_img)     #그냥 흑색화면
            listxp1 =[]
            listxp2 =[]
            listyp1 = []
            listyp2 = []
            listxm1 =[]
            listxm2 =[]
            listym1 = []
            listym2 = []
            #print(len(lines))           #일순간의 허프변환을 통했을때 그 순간의 선의 길이?
            if lines is not None:
                for i in range(0, len(lines)):
                    # x1,y1,x2,y2 -> 직선의 좌표구하는것(x1,y1) ~ (x2,y2)
                    r, theta = lines[i][0]  #허피 변환을 하면 직선이 나올수 있는 모든 경우의 수의 기울기를 뽑아줌
                    a = np.cos(theta)
                    b = np.sin(theta)
                    x0 = a* r
                    y0 = b* r
                    x1 = int(x0 + 1000*(-b))
                    y1 = int(y0 + 1000*(a))
                    x2 = int(x0 - 1000*(-b))
                    y2 = int(y0 - 1000*(a))
                    # 간혹 직선의 경우의수가 안뽑힐 때도있음, 실제 테스트 해보고 캐니엣지를 건드려줘야함 이럴때

                    try:
                        cv2.line(lines_img,(x1,y1),(x2,y2),(0,255,0),2)
                        if y1 > 0:
                            slope = abs(y2 - y1) / abs(x1 - x2)
                        else:
                            slope = abs(y1 - y2) / abs(x1 - x2)
                        if 0.2 < slope < 30:  # 1차적으로 쓸모없고 평균값 잡아먹는 말도안되는 기울기를 가진 직선을 1차적으로 필터링해줌
                        #if (y0 > 100 and x0 > 100) or (y0 < 100 and x0 < 100):
                            if y1 > 0:  # 이안에 들어있으면 우선 Y1과 Y2를 판별부터함 이게 섞여있어서 Y1이 양수일 때도 있고 음수일때도있음
                                # x 축은 무조건 x1  = -값 x2는 무조건 +값이라서 y1이 양수면 2사분면에서 4사분면, y1이 음수면 3사분면에서 1사분면으로 이동
                                # 이거를 따로따로 리스트에 전부 구해서 추가시켜줄거임
                                listxm1.append(x2)
                                listxm2.append(x1)
                                listym1.append(y2)
                                listym2.append(y1)
                                cv2.line(lines_img,(x1,y1),(x2,y2),(0,0,255),2)
                            else:
                                listxp1.append(x1)
                                listxp2.append(x2)
                                listyp1.append(y1)
                                listyp2.append(y2)
                                cv2.line(lines_img,(x1,y1),(x2,y2),(255,0,0),2)
                            #print(f' i = {i} x1 = {x1}, x2 = {x2} , y1 = {y1}, y2 = {y2} , y0 = {y0}, x0 = {x0}')
                            #print(f' i = {i} x1 = {x1}, x2 = {x2} , y1 = {y1}, y2 = {y2}')  여기
                        

                    except ZeroDivisionError:   #직선이 되는 경우의수가 없는경우 그냥 건너뜀 어차피 while문안이고, 개빠르게 계산하기 때문에 금방 넘어감
                        t1 = time.time() - 0.2
                        t2 = time.time()
                        listxp1 = []
                        listxp2 = []
                        listyp1 = []
                        listyp2 = []
                        listxm1 = []
                        listxm2 = []
                        listym1 = []
                        listym2 = []
                        continue
                try:
                    px1 = int(np.mean(listxp1))  #리스트에잇는것들 다 평균값 구함     #p로시작하는게 왼쪽직선  밑에m으로 시작하는게 오른쪽직선
                    px2 = int(np.mean(listxp2))
                    py1 = int(np.mean(listyp1))
                    py2 = int(np.mean(listyp2))
                    cv2.line(mask, (px1, py1), (px2, py2), (255, 255, 255), 2)
                    #cv2.line(src, (px1, py1), (px2, py2), (255, 255, 255), 2)
                    mx1 = int(np.mean(listxm1))
                    mx2 = int(np.mean(listxm2))
                    my1 = int(np.mean(listym1))
                    my2 = int(np.mean(listym2))
                    cv2.line(mask, (mx1, my1), (mx2, my2), (255, 255, 255), 2)
                    #cv2.line(src, (mx1, my1), (mx2, my2), (255, 255, 255), 2)
                    Pa = (py2 - py1) / (px2 - px1)  #직선의 방정식을 이용하여 소실점 구하는 공식       좌측 직선 기울기
                    Ma = (my2 - my1) / (mx2 - mx1)  #직선의 방정식을 이용하여 소실점 구하는 공식       우측 직선 기울기

                    #print(int(Pa*100))      #44,-85일때 직진중
                    #print(int(Ma*100))
                    xt = int(((Pa * px1)-py1 - (Ma*mx1) + my1) / (Pa - Ma))  #직선의 방정식을 이용하여 소실점 구하는 공식      #이 xt yt가 소실점
                    yt = int((Pa * (xt - px1)) + py1)  #직선의 방정식을 이용하여 소실점 구하는 공식
                    #print(xt)
                    #print(yt)
                    cv2.line(src, (320, 0), (320, 360), (0, 0, 255), 4)     #중앙선(이거는 우리가 설정하는 값이므로 상수형태로 그냥 썼다)
                    car = motorAction(xt,yt)
                    direction = car.whereControl()
                    #if previous != direction:
                        #flag = 1
                    #else:
                        #flag = 0
                    #previous = direction
                    #print("direction = %s, flag = %s" %(direction,flag))
                    cv2.line(src,(xt,yt),(xt,yt),(255,0,0),9)         #소실점  영상이 한쪽에 치우쳐져있어서 임시로 40을 저한거임
                except ValueError:   #직선이되는 경우의 수가 없어 zerodivision 오류가떠서 pass가 났다. 그럼 리스트에 아무 값도 들어있지 않아서 이것도 말이안됨 따라서, 이부분도 패스시켜줌
                    pass
            t1 = time.time()
            t2 = time.time()
            cv2.imshow("ROI",lines_img)
            #cv2.imshow('cdstP',cdstP)
            cv2.imshow('source',mask)
            previous = 1
        else:
            t2 = time.time()
    if previous == 1 and customControlServer.OnOff == False:
        cv2.destroyWindow("ROI")
        cv2.destroyWindow('source')
        previous = 0
    cv2.imshow('video', src)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
"""
