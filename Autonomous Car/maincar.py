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
    global leftpoint,rightpoint,rcount,lcount,srcount,slcount
    command=request.forms.get('command')
    print (command)
    if command == "AUTO":
        auto()
        leftpoint = 0
        rightpoint = 0
        rcount=0
        lcount=0
        srcount=0
        slcount=0
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
rcount=0
lcount=0
srcount=0
slcount=0
lrspeed=1
slrspeed=0.2

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

#cap = cv2.VideoCapture(0) #연결된 웹캠으로 비디오 연결(0)
cap2 = cv2.VideoCapture(-1)#maybe this is piCam
cap = cv2.VideoCapture(1)#maybe this is webCam

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
autoModule = def2(name = "2")
zodoModule.start()
liderModule.start()
autoModule.start()
previous = 0
leftpoint = 0
rightpoint = 0
lflag = 0
rflag = 0
a = 0
b = 0
#traffic======================================
hsv = 0
lower_blue1 = 0
upper_blue1 = 0
lower_blue2 = 0
upper_blue2 = 0
lower_blue3 = 0
upper_blue3 = 0
Traffic_Light = False  # False일때는 빨간불찾기모드 // True는 파란불찾기 모드 // 처음엔 일단 직전으로시작하므로 일단은 빨간불부터 찾아야한다.
refresh = []
cnt = 0
stopflag = 0
#=============================================

t1 = time.time()
while (True):
    if t2 == None:
        t2 = time.time()
    ret, src = cap.read() #src는 프레임임(원본이미지)
    ret2, src2 = cap2.read()#piCam is cap2 we gonna use this to trafficLight
    
    src=cv2.resize(src,(640,360)) #가로640 세로 360
    src2=cv2.resize(src2,(640,360)) #가로640 세로 360
    
    #traffic============================================
    Traffic_Light_ROI = src2[0:250, 0:340]
    img_hsv = cv2.cvtColor(Traffic_Light_ROI, cv2.COLOR_BGR2HSV)
    if Traffic_Light == True:  # 파란불 찾기 모드일때
        # 이거는 초록색(그냥 상수로 넣어도 된다)
        img_mask1 = cv2.inRange(img_hsv, (68, 30, 30), (78, 255, 255))
        img_mask2 = cv2.inRange(img_hsv, (56, 30, 30), (54, 255, 255))
        img_mask3 = cv2.inRange(img_hsv, (56, 30, 30), (68, 255, 255))

        #img_mask1_big = cv.inRange(img_hsv_big, (68, 30, 30), (78, 255, 255))
        #img_mask2_big = cv.inRange(img_hsv_big, (56, 30, 30), (54, 255, 255))
        #img_mask3_big = cv.inRange(img_hsv_big, (56, 30, 30), (68, 255, 255))
    else:  # 빨간불 찾기 모드일때
        img_mask1 = cv2.inRange(img_hsv, (140, 30, 30), (180, 255, 255))
        img_mask2 = cv2.inRange(img_hsv, (1, 30, 30), (5, 255, 255))
        img_mask3 = cv2.inRange(img_hsv, (100, 30, 30), (15, 255, 255))

        # img_mask1_big = cv.inRange(img_hsv_big, (68, 30, 30), (78, 255, 255))
        # img_mask2_big = cv.inRange(img_hsv_big, (56, 30, 30), (54, 255, 255))
        # img_mask3_big = cv.inRange(img_hsv_big, (56, 30, 30), (68, 255, 255))
    img_mask = img_mask1 | img_mask2 | img_mask3

    kernel = np.ones((11, 11), np.uint8)
    img_mask = cv2.morphologyEx(img_mask, cv2.MORPH_OPEN, kernel)  # 모폴로지-영상노이즈제거
    img_mask = cv2.morphologyEx(img_mask, cv2.MORPH_CLOSE, kernel)
    img_result = cv2.bitwise_and(Traffic_Light_ROI, Traffic_Light_ROI, mask=img_mask)
    numOfLabels, img_label, stats, centroids = cv2.connectedComponentsWithStats(img_mask)
    for idx, centroid in enumerate(centroids):  # enumerate는 리스트 값을 전달하는 기능
        if stats[idx][0] == 0 and stats[idx][1] == 0:
            continue

        if np.any(np.isnan(centroid)):
            continue

        x, y, width, height, area = stats[idx]
        centerX, centerY = int(centroid[0]), int(centroid[1])
        # print(centerX, centerY)

        if area > 50:
            #cv.circle(img_color, (centerX, centerY), 10, (0, 0, 255), 10)
            #cv.rectangle(img_color, (x, y), (x + width, y + height), (0, 0, 255))
            cv2.circle(Traffic_Light_ROI, (centerX, centerY), 10, (0, 0, 255), 10)
            cv2.rectangle(Traffic_Light_ROI, (x, y), (x + width, y + height), (0, 0, 255))


    if 0 < centroid[0] and centroid[0] <=340 and centroid[0] != 169.5:         #인식된 색의 x좌표가 roi영역의 x축 안에있으면
        if centroid[1] <=250 and centroid[1] != 124.5:                          #인식된 색의 y좌표가 roi영역의 y축 안에있으면
            cnt+=1
            print(cnt)
            if cnt >= 30:   #50되면
                print("Flag Toggle")
                Traffic_Light = not Traffic_Light
                cnt = 0
                
        else:
            cnt = 0
    else:
        cnt = 0
    #=============================================
        
    if customControlServer.OnOff == True:
        if (Traffic_Light == True or lider.liderflag == 1):
            stopflag = 1
        else:
            stopflag = 0
        if t2 - t1 >= 0.01:
            point = 0
            src_clone = cv2.GaussianBlur(src, (7,7), 0)
            cdst = cv2.Canny(src_clone, 30, 200, None, 3) # dst를 캐니 여기서 괄호는 각각(사진, 임계값1,임계값2) 이다 임계값 1이하면 가장자리 제외
            #cdst = cv.cvtColor(dst, cv.COLOR_GRAY2BGR) #캐니엣지 처리된 dst를 그레이 스케일로
            cdstP = np.copy(cdst)#cdst의 똑같은 크기의 사진(배열)을 만듦

            #### 관심영역(ROI) 지정하는 부분
            imshape = src.shape
            vertices = np.array([[(0,200),
                                  (640, 200),
                                  (640, 360),
                                  (0, 360)]])
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
            customControlServer.velocity = 0.35
            #0.2
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
                        if 0.1 < slope < 100:  # 1차적으로 쓸모없고 평균값 잡아먹는 말도안되는 기울기를 가진 직선을 1차적으로 필터링해줌
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
                except ValueError:   #직선이되는 경우의 수가 없어 zerodivision 오류가떠서 pass가 났다. 그럼 리스트에 아무 값도 들어있지 않아서 이것도 말이안됨 따라서, 이부분도 패스시켜줌
                    point = 1
                try:
                    mx1 = int(np.mean(listxm1))
                    mx2 = int(np.mean(listxm2))
                    my1 = int(np.mean(listym1))
                    my2 = int(np.mean(listym2))
                    if point == 1:
                        xt = (mx1 + mx2) / 2
                        yt = (my1 + my2) / 2
                        if xt < 180:
                            xt = xt+ 180
                    cv2.line(mask, (mx1, my1), (mx2, my2), (255, 255, 255), 2)
                    #cv2.line(src, (mx1, my1), (mx2, my2), (255, 255, 255), 2)
                except ValueError:   #직선이되는 경우의 수가 없어 zerodivision 오류가떠서 pass가 났다. 그럼 리스트에 아무 값도 들어있지 않아서 이것도 말이안됨 따라서, 이부분도 패스시켜줌
                    try:
                        xt = (px1 + px2) / 2
                        yt = (py1 + py2) / 2
                        if xt>180:
                            xt = xt - 180
                    except ValueError:
                        direction = previous
                        pass
                try:
                    Pa = (py2 - py1) / (px2 - px1)  #직선의 방정식을 이용하여 소실점 구하는 공식       좌측 직선 기울기
                    Ma = (my2 - my1) / (mx2 - mx1)  #직선의 방정식을 이용하여 소실점 구하는 공식       우측 직선 기울기

                    #print(int(Pa*100))      #44,-85일때 직진중
                    #print(int(Ma*100))
                    xt = int(((Pa * px1)-py1 - (Ma*mx1) + my1) / (Pa - Ma))  #직선의 방정식을 이용하여 소실점 구하는 공식      #이 xt yt가 소실점
                    yt = int((Pa * (xt - px1)) + py1)  #직선의 방정식을 이용하여 소실점 구하는 공식
                    #print(xt)
                    #print(yt)
                    cv2.line(src, (320, 0), (320, 360), (0, 0, 255), 4)     #중앙선(이거는 우리가 설정하는 값이므로 상수형태로 그냥 썼다)
                    #if previous != direction:
                        #flag = 1
                    #else:
                        #flag = 0
                    #previous = direction
                    #print("direction = %s, flag = %s" %(direction,flag))
                    cv2.line(src,(xt,yt),(xt,yt),(255,0,0),9)         #소실점  영상이 한쪽에 치우쳐져있어서 임시로 40을 저한거임
                except ValueError:   #직선이되는 경우의 수가 없어 zerodivision 오류가떠서 pass가 났다. 그럼 리스트에 아무 값도 들어있지 않아서 이것도 말이안됨 따라서, 이부분도 패스시켜줌
                    pass
                except NameError:   #직선이되는 경우의 수가 없어 zerodivision 오류가떠서 pass가 났다. 그럼 리스트에 아무 값도 들어있지 않아서 이것도 말이안됨 따라서, 이부분도 패스시켜줌
                    pass
            car = motorAction(xt,yt)
            direction = car.whereControl()
            if len(listxm1) == 0:
                leftpoint = 2
                lflag = 0
            elif len(listxp1) == 0:
                rightpoint = 2
                rflag = 0
            if stopflag == 1:
                customControlServer.stop()
            #if direction >= -4 and direction <= 4 and leftpoint ==0 and rightpoint == 0:
            elif direction >= -4 and direction <= 3 and leftpoint ==0 and rightpoint == 0:
                '''
                if -5 < direction <= -2:
                    slcount += 1
                    if(slcount == 3):
                        customControlServer.forward()
                        time.sleep(0.15)
                        customControlServer.left(lrspeed)
                        time.sleep(0.3)
                        customControlServer.slowLeft()
                        time.sleep(0.2)
                        customControlServer.stop()
                        print("slow left")
                        slcount = 0
                elif 2 <= direction < 5:
                    srcount += 1
                    if(srcount == 3):
                        customControlServer.forward()
                        time.sleep(0.15)
                        customControlServer.right(lrspeed)
                        time.sleep(0.3)
                        customControlServer.slowRight()
                        time.sleep(0.2)
                        customControlServer.stop()
                        print("slow right")
                        srcount = 0
                else:
                '''
                customControlServer.velocity = customControlServer.velocity + 0.2
                customControlServer.forward()
                time.sleep(0.1)
                customControlServer.velocity = customControlServer.velocity - 0.2
                customControlServer.forward()
                print("go")
            elif direction <-4 or leftpoint != 0:
                lcount += 1
                if(lcount >= 2):
                    if leftpoint == 0:
                        customControlServer.stop()
                        leftpoint = 1
                    if leftpoint == 1:
                        if len(listxm1) == 0:
                            print("left1")
                            customControlServer.forward()
                            time.sleep(0.25)
                            customControlServer.left(lrspeed)
                            time.sleep(0.1)
                            customControlServer.stop()
                            a = len(listxp1)
                        elif len(listxm1) != 0 or len(listxp1) > a + 2:
                            lcount  = lcount + 1
                            if lcount >= 2:
                                leftpoint = 0
                                rightpoint = 0
                                lcount = 0
                                lflag = 1
                            else:
                                print("left3")
                                customControlServer.forward()
                                time.sleep(0.25)
                                customControlServer.left(lrspeed)
                                time.sleep(0.1)
                                customControlServer.stop()
                        elif direction >3:
                            leftpoint = 1
                            rightpoint = 0
                        else:
                            print("left2")
                            customControlServer.forward()
                            time.sleep(0.25)
                            customControlServer.left(lrspeed)
                            time.sleep(0.1)
                            customControlServer.stop()
            elif direction > 3 or rightpoint != 0:
                rcount += 1
                if(rcount >= 4):
                    if rightpoint == 0:
                        customControlServer.stop()
                        rightpoint = 1
                    if rightpoint == 1:
                        if len(listxp1) == 0:
                            print("right1")
                            customControlServer.forward()
                            time.sleep(0.1)
                            customControlServer.right(lrspeed)
                            time.sleep(0.1)
                            customControlServer.stop()
                            b = len(listxm1)
                        elif len(listxp1) != 0 or len(listxm1) > b + 2:
                            rcount  = rcount + 1
                            if rcount >= 2:
                                leftpoint = 0
                                rightpoint = 0
                                rcount = 0
                                rflag = 1
                            else:
                                print("right3")
                                customControlServer.forward()
                                time.sleep(0.1)
                                customControlServer.right(lrspeed)
                                time.sleep(0.1)
                                customControlServer.stop()
                        elif direction < -3:
                            leftpoint = 0
                            rightpoint = 1
                        else:
                            print("right2")
                            customControlServer.forward()
                            time.sleep(0.1)
                            customControlServer.right(lrspeed)
                            time.sleep(0.1)
                            customControlServer.stop()
            print('sinhodong =' +  str(Traffic_Light) + 'rider' + str(lider.liderflag))
            print("leftpoint" + str(len(listxp1)) + "rightpoint" + str(len(listxm1)) )
            t1 = time.time()
            t2 = time.time()
            cv2.imshow("ROI",lines_img)
            #cv2.imshow('cdstP',cdstP)
            cv2.imshow('source',mask)
            previous = direction
        else:
            t2 = time.time()
    if previous == 1 and customControlServer.OnOff == False:
        cv2.destroyWindow("ROI")
        cv2.destroyWindow('source')
        previous = 0
    cv2.imshow('Traffic_Light_ROI', Traffic_Light_ROI)
    cv2.imshow('video', src)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()




