

# 1. Overview
* 기술을 통해 사람의 감성과 감성을 이어주는 기업, 이모션웨이브와 함께 진행한 프로젝트
* midi프로그램으로 부터 악보를 추출하여 마림바 스틱이 장착된 액추에이터로 실제 연주를 하는 모듈 SW 제작
****
# 2. Structure
## 2.1. 프로그램 프름도
![자율주행차흐름도](https://user-images.githubusercontent.com/52540882/116738808-e2c8e900-aa2d-11eb-9172-7974a5c039d8.PNG)
   
****
# 3. Language/Tools
## 3.1. Language
* C++
## 3.2. Tools
* Arduino Uno, Arduino Promicro,  Arduino IDE
* CakeWalk midi, Diylc
****
   
# 4. Result
## 4.1 Midi 프로그램과 Arduino Promicro간의 무선 통신
이미지
* 미디 프로그램에서 받은 음표 신호를 콘솔을 통해 확인      
![주행 알고리즘](https://user-images.githubusercontent.com/52540882/116739838-2112d800-aa2f-11eb-9afd-f58211392558.PNG)   
* 미디 프로그램에서 받은 음표 신호를 간단한 LED 회로를 통해서 확인 가능    

## 4.2 만능기판 솔더링을 통한 회로 제작
![차선검출](https://user-images.githubusercontent.com/52540882/116739461-ae096180-aa2e-11eb-84eb-50b906ef0d3b.gif)   
* 패키징 별도
     
## 4.2 액추에이터 2개를 통한 동작 시험
이미지
* 2개의 모터에 대한 테스트 후 추가 제작 결정

****
# 5. Problem/Improve
## 5.1. 아두이노 SRAM 용량 초과
* 각 액추에이터별 하강/상승 동작의 시간을 long int형으로 각각 제어를 시도했더니 보드의 SRAM 최대 용량인 2KB를 초과하는 이슈가 있었다.
