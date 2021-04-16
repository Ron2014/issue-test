/*
 * @Author: Ron
 * @LastEditors: Ron
 * @FilePath: \issue-test\c++\test45\define.h
 */

#include <iostream>
#include "MyStr.h"

typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned char UINT8;
typedef char INT8;
typedef std::string FString;

struct BattleRecordDetails
{
    UINT64 RecordID;
    UINT32 AccountID;
    bool ApplyFlag;      //0没有申请 1已发申请
    UINT8 KnockOutCount; //淘汰数 击杀数
    UINT32 DamageValue;
    UINT32 GameTime;   //游戏时长
    UINT8 ReviveCount; //救助数量
    UINT32 CureValue;
    UINT32 SkillCastCount; //技能释放次数
    bool MVP;
    INT8 HeroId;
    FString Name;
};

struct BattleRecordDetailsShm
{
    UINT64 RecordID;
    UINT32 AccountID;
    bool ApplyFlag;      //0没有申请 1已发申请
    UINT8 KnockOutCount; //淘汰数 击杀数
    UINT32 DamageValue;
    UINT32 GameTime;   //游戏时长
    UINT8 ReviveCount; //救助数量
    UINT32 CureValue;
    UINT32 SkillCastCount; //技能释放次数
    bool MVP;
    INT8 HeroId;
    CMyStr<CMyStr_Size_32> Name;

    bool operator=(BattleRecordDetails &stBattleRecord)
    {
        this->RecordID = stBattleRecord.RecordID;
        this->AccountID = stBattleRecord.AccountID;
        this->ApplyFlag = stBattleRecord.ApplyFlag;
        this->KnockOutCount = stBattleRecord.KnockOutCount;
        this->DamageValue = stBattleRecord.DamageValue;
        this->GameTime = stBattleRecord.GameTime;
        this->ReviveCount = stBattleRecord.ReviveCount;
        this->CureValue = stBattleRecord.CureValue;
        this->SkillCastCount = stBattleRecord.SkillCastCount;
        this->MVP = stBattleRecord.MVP;
        this->HeroId = stBattleRecord.HeroId;
        this->Name = stBattleRecord.Name;
    }
};