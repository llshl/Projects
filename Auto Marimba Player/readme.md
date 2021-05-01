

# 1. Overview
* 기술을 통해 사람의 감성과 감성을 이어주는 기업, 이모션웨이브와 함께 진행한 프로젝트
* midi프로그램으로 부터 악보를 추출하여 마림바 스틱이 장착된 액추에이터로 실제 연주를 하는 모듈 SW 제작
****
# 2. Structure
## 2.1. 프로그램 순서도
![마림바 흐름도](https://user-images.githubusercontent.com/52540882/116779905-059de080-aab4-11eb-96ee-aabe680f59f6.PNG)
   
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
![신호받기gif](https://user-images.githubusercontent.com/52540882/116780616-c3c36900-aab8-11eb-8e79-b20d60355e9e.gif)
![신호받기1](https://user-images.githubusercontent.com/52540882/116780337-0ab05f00-aab7-11eb-9150-bd82fcb4a19b.png)
* 미디 프로그램에서 받은 음표 신호를 콘솔을 통해 확인   

## 4.2 액추에이터 2개를 통한 동작 시험
![마림바 연주움짤](https://user-images.githubusercontent.com/52540882/116780741-6aa80500-aab9-11eb-925d-dd9dd246f97b.gif)
* 2개의 모터에 대한 테스트 후 추가 제작 결정

****
# 5. Problem/Improve
## 5.1. 아두이노 SRAM 용량 초과
![메모리초과](https://user-images.githubusercontent.com/52540882/116779903-033b8680-aab4-11eb-999d-4c30066a7840.png)   
* 각 액추에이터별 하강/상승 동작의 시간을 long int형으로 각각 제어를 시도했더니 보드의 SRAM 최대 용량인 2KB를 초과하는 이슈가 있었다.
