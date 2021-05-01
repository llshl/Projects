import cv2
import numpy as np
# traffic======================================
hsv = 0
lower_blue1 = 0
upper_blue1 = 0
lower_blue2 = 0
upper_blue2 = 0
lower_blue3 = 0
upper_blue3 = 0
refresh = []
stopflag = 0
# =============================================
def TrafficLight(ret2,src2,Traffic_Light):
    cnt = 0
    # traffic============================================
    Traffic_Light_ROI = src2[0:250, 0:340]
    img_hsv = cv2.cvtColor(Traffic_Light_ROI, cv2.COLOR_BGR2HSV)
    if Traffic_Light == True:  # 파란불 찾기 모드일때
        # 이거는 초록색(그냥 상수로 넣어도 된다)
        img_mask1 = cv2.inRange(img_hsv, (68, 30, 30), (78, 255, 255))
        img_mask2 = cv2.inRange(img_hsv, (56, 30, 30), (54, 255, 255))
        img_mask3 = cv2.inRange(img_hsv, (56, 30, 30), (68, 255, 255))

        # img_mask1_big = cv.inRange(img_hsv_big, (68, 30, 30), (78, 255, 255))
        # img_mask2_big = cv.inRange(img_hsv_big, (56, 30, 30), (54, 255, 255))
        # img_mask3_big = cv.inRange(img_hsv_big, (56, 30, 30), (68, 255, 255))
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
            # cv.circle(img_color, (centerX, centerY), 10, (0, 0, 255), 10)
            # cv.rectangle(img_color, (x, y), (x + width, y + height), (0, 0, 255))
            cv2.circle(Traffic_Light_ROI, (centerX, centerY), 10, (0, 0, 255), 10)
            cv2.rectangle(Traffic_Light_ROI, (x, y), (x + width, y + height), (0, 0, 255))

    if 0 < centroid[0] and centroid[0] <= 340 and centroid[0] != 169.5:  # 인식된 색의 x좌표가 roi영역의 x축 안에있으면
        if centroid[1] <= 250 and centroid[1] != 124.5:  # 인식된 색의 y좌표가 roi영역의 y축 안에있으면
            cnt += 1
            print(cnt)
            if cnt >= 30:  # 50되면
                print("Flag Toggle")
                Traffic_Light = not Traffic_Light
                cnt = 0

        else:
            cnt = 0
    else:
        cnt = 0
    return Traffic_Light