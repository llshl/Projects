
# 1. Overview
* 4차 산업혁명을 맞이하여 급박하게 변하고 있는 분야는 바로 자동차이다. 세계보건기구(WHO)의 발표에 따르면 전 세계에서 매일 수백만 명이 교통사고로 다치고 3500명 이상이 사망한다. 미래의 자동차에는 정교하고 검증받은 자율주행 기술이 필수적인 요소가 될 것이다.   
* Python과 OpenCV을 사용하여 차선을 검출해내고 동시에 신호등의 상황을 인지하여 자율적으로 주행 의사결정을 하는 라즈베리파이 기반 자율주행 RC카를 제작했다. 또한 간단한 조작 UI를 제공하는 웹페이지를 통해 수동 제어가 가능하도록 구현하였고 라이다센서와 조도센서를 통한 거리조절 및 전조등 점등을 구현했다.

# 2. Structure
## 2.1. 프로그램 프름도
![자율주행차흐름도](https://user-images.githubusercontent.com/52540882/116738808-e2c8e900-aa2d-11eb-9172-7974a5c039d8.PNG)


## 2.2 자율주행/수동주행 모드간의 전환 기준
![자동모드와수동모드의 전환 순서도](https://user-images.githubusercontent.com/52540882/116740536-202e7600-aa30-11eb-9e55-5841b43a6e7b.PNG)


****
# 3. Language/Tools
## 3.1. Language
* Python
* Html, Css, Javascript
## 3.2. Tools
* PC - PythonCharm IDE
* Raspberry Pi - Python IDLE
* OpenCV library

# 4. Result
## 4.1 차선검출을 통한 방향 제어
![주행 알고리즘](https://user-images.githubusercontent.com/52540882/116739838-2112d800-aa2f-11eb-9afd-f58211392558.PNG)   
* 주행 알고리즘         
![차선검출](https://user-images.githubusercontent.com/52540882/116739461-ae096180-aa2e-11eb-84eb-50b906ef0d3b.gif)   
* 차선검출/소실점 추출            
![ezgif com-gif-maker](https://user-images.githubusercontent.com/52540882/116738951-1572e180-aa2e-11eb-8c3c-3c9cb2ea5c9a.gif)    
* 차선을 따라 자율주행   

## 4.2 신호등 색 검출을 통한 주행/정지
![신호등검출](https://user-images.githubusercontent.com/52540882/116740082-764ee980-aa2f-11eb-8cf9-b76617fbe19d.png)   
* 파란불 신호를 인식하여 box표시
 
## 4.3 직관적인 UI를 사용한 수동조작
![조작 UI](https://user-images.githubusercontent.com/52540882/116739074-3804fa80-aa2e-11eb-9a94-ecd8fa78f98e.jpg)
* Python Bottle Framework를 통한 수동 조작 UI 웹페이지 배포

# 4. Problem/Improve
## 4.1. 라즈베리파이의 연산량 한계 초과 이슈
* 프레임수가 높은 고성능의 웹캠에서 입력되는 연산 데이터들을 라즈베리파이가 감당하지 못하여서 주행이 뚝뚝 끊기며 부자연스럽게 동작하였다.
* 차선검출 로직의 main loop문에서 걸리는 부하를 해결하기위해 딜레이 함수를 통해 초당 처리 횟수를 줄였고 어느정도 움직임을 개선할 수 있었다.
* GPU가 탑제된 영상처리 특화 MPU를 사용했다면 자연스러운 주행할 것으로 예상된다.

## 4.2. 자동차 스티어링의 부재
* 방향 전환을 함에 있어서 스티어링이 없었기에 모든 바퀴의 회전각을 통해 방향을 전환할 수 밖에 없었다.
* 스티어링이 구현된 자동차였다면 훨씬 간결한 코드로와 더욱 깔끔한 움직임이 가능할 것으로 예상된다.

