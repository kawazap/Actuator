#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include "Motor.h"

//使うピンの定義
#define  Airbale 4
#define  Airrale 5
#define  door  2
#define  rale 3
#define  flag 30
Motor md(A2,A3);//扉
Motor mr(A0,A1);//レール

//ノードハンドルの宣言
ros::NodeHandle nh;

std_msgs::String chat;
std_msgs::Bool try_msg;

ros::Publisher ACtry("ACtry", &try_msg);//ボールをおいたかどうかの判断
ros::Publisher CHpub("ACmode", &chat);//changeでのmodeを表示(削除可)

//bool
bool doorpub = true;
bool ralepub =true;

//timer
long last_time=0;
long last_back_time=0;

//door:L,rale:Lが二回あるので、パターンわけする
int pattern = 0;

//mode切り替えに使う関数
void  change(const std_msgs::Int32& ac_msg){
    if( ac_msg.data == 0){//初期位置
      digitalWrite(Airbale, LOW);
       digitalWrite(flag,LOW);
       chat.data = "before";
    }else if( ac_msg.data == 1){//初期位置でボールをつかむ
      digitalWrite(Airbale, HIGH);
       digitalWrite(flag,LOW);
       chat.data = "aftor";
    }else if( ac_msg.data == 2){//tryzoneでの動き
      digitalWrite(flag,HIGH); 
      chat.data = "try";
    }
    CHpub.publish(&chat);
}
// トピック名をchangeとしてSubscriberをインスタンス化し、changeを呼び出す
ros::Subscriber<std_msgs::Int32> CHsub("change", &change );

void Actuator(){
  digitalWrite(Airbale,HIGH);
  delay(1000);
    digitalWrite(Airbale,LOW);
  delay(1000);//デバッグ用*/
 /* static uint32_t pre_time;
   if(digitalRead(rale)==HIGH){
    ralepub = false;
       if(digitalRead(door) == LOW){//Door:H,Rale:H
            mr.run(0);
            md.run(0);
        }else{//Door:L,Rale:H
          if(millis() - last_time > 3000){
           digitalWrite(Airbale, LOW);
            mr.run(0);
            md.run(0);
          }
         if(millis() - last_time > 5000 ){
          mr.run(200);
          md.run(0);
         }
          last_back_time = millis();
          pattern = pattern + 1;
        }
   
   }else{
    if(digitalRead(door) == LOW){//Door:H,Rale:L
      ralepub = false;
      md.run(200);
      mr.run(0);
      pattern = 0;
      last_time = millis();
    }else{//Door:L,Rale:L
      if(pattern % 2 == 0){
       digitalWrite(Airrale, HIGH);
        mr.run(0);
        md.run(0);
       ralepub = true;
      }else if(pattern % 2  == 1){
        ralepub = false;
       digitalWrite(Airrale, LOW);
       if((millis() - last_back_time) <3000){
        mr.run(200);//次に備えて糸を出す
        md.run(0);
       }
       if(digitalRead(door) == HIGH){
        md.run(200);
        mr.run(0);
       }
      }
    }
   }

 if (millis()-pre_time >= 50)
  {
    try_msg.data =  ralepub;
    ACtry.publish(&try_msg);
    pre_time = millis();
  }*/
}

void setup()
{ 
  nh.initNode();
  nh.subscribe(CHsub);
  nh.advertise(CHpub);
  nh.advertise(ACtry);
  for(int i=2;i<13;i++)
  {
    pinMode(i,OUTPUT); 
  }
  pinMode(flag, INPUT);
   pinMode(door, INPUT);
    pinMode(rale, INPUT);
  digitalWrite(door, HIGH);
  digitalWrite(rale, LOW);
  digitalWrite(flag,LOW);
   pattern = 0;
}

void loop()
{  
  if(digitalRead(flag) == HIGH){//whileはダメ
    Actuator();
  }
  nh.spinOnce();
  delay(1);
}
