#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <cstdint>
#include <array>
#include <map>

namespace THUAI6
{

    // 游戏状态
    enum class GameState : unsigned char
    {
        NullGameState = 0,
        GameStart = 1,
        GameRunning = 2,
        GameEnd = 3,
    };
    // 所有NullXXXType均为错误类型，其余为可能出现的正常类型

    // 位置标志
    enum class PlaceType : unsigned char
    {
        NullPlaceType = 0,
        Land = 1,
        Wall = 2,
        Grass = 3,
        ClassRoom = 4,
        Gate = 5,
        HiddenGate = 6,
        Window = 7,
        Door = 8,
        Chest = 9,
    };

    // 形状标志
    enum class ShapeType : unsigned char
    {
        NullShapeType = 0,
        Circle = 1,
        Square = 2,
    };

    // 道具类型
    enum class PropType : unsigned char
    {
        NullPropType = 0,
        PropType1 = 1,
        PropType2 = 2,
        PropType3 = 3,
        PropType4 = 4,
    };

    enum class BulletType : unsigned char
    {
        NullBulletType = 0,
        LineBullet = 1,
        CommonBullet = 2,
        FastBullet = 3,
        OrdinaryBullet = 4,
        AtomBomb = 5,
    };

    // 玩家类型
    enum class PlayerType : unsigned char
    {
        NullPlayerType = 0,
        StudentPlayer = 1,
        TrickerPlayer = 2,
    };

    // 学生类型
    enum class StudentType : unsigned char
    {
        NullStudentType = 0,
        StudentType1 = 1,
        StudentType2 = 2,
        StudentType3 = 3,
        StudentType4 = 4,
    };

    // 捣蛋鬼类型
    enum class TrickerType : unsigned char
    {
        NullTrickerType = 0,
        TrickerType1 = 1,
        TrickerType2 = 2,
        TrickerType3 = 3,
        TrickerType4 = 4,
    };

    // 学生Buff类型
    enum class StudentBuffType : unsigned char
    {
        NullStudentBuffType = 0,
        StudentBuffType1 = 1,
        StudentBuffType2 = 2,
        StudentBuffType3 = 3,
        StudentBuffType4 = 4,
    };

    enum class TrickerBuffType : unsigned char
    {
        NullTrickerBuffType = 0,
        TrickerBuffType1 = 1,
        TrickerBuffType2 = 2,
        TrickerBuffType3 = 3,
        TrickerBuffType4 = 4,
    };

    // 学生状态枚举
    enum class PlayerState : unsigned char
    {
        NullState = 0,
        Idle = 1,
        Learning = 2,
        Addicted = 3,
        Quit = 4,
        Graduated = 5,
        Treated = 6,
        Rescued = 7,
        Stunned = 8,
        Treating = 9,
        Rescuing = 10,
        Swinging = 11,
        Attacking = 12,
        Locking = 13,
        Rummaging = 14,
        Climbing = 15,
    };

    enum class MessageOfObj : unsigned char
    {
        NullMessageOfObj = 0,
        StudentMessage = 1,
        TrickerMessage = 2,
        PropMessage = 3,
        BulletMessage = 4,
        BombedBulletMessage = 5,
        ClassroomMessage = 6,
        DoorMessage = 7,
        GateMessage = 8,
        ChestMessage = 9,
        MapMessage = 10,
        NewsMessage = 11,
        HiddenGateMessage = 12,
    };

    // 玩家类
    struct Player
    {
        int32_t x;          // x坐标
        int32_t y;          // y坐标
        int32_t speed;      // 移动速度
        int32_t viewRange;  // 视野范围
        int64_t playerID;   // 玩家ID
        int64_t guid;       // 全局唯一ID
        int16_t radius;     // 圆形物体的半径或正方形物体的内切圆半径
        int32_t score;      // 分数

        double facingDirection;  // 朝向

        std::vector<double> timeUntilSkillAvailable;  // 技能冷却时间

        PlayerType playerType;  // 玩家类型
        std::vector<PropType> props;
        PlaceType place;  // 所处格子的类型
        BulletType bulletType;

        PlayerState playerState;
    };

    struct Student : public Player
    {
        StudentType studentType;
        int32_t determination;  // 剩余毅力
        int32_t addiction;      // 沉迷程度
        int32_t learningSpeed;
        int32_t treatSpeed;
        int32_t treatProgress;
        int32_t rescueProgress;
        double dangerAlert;
        std::vector<StudentBuffType> buff;  // buff
    };

    struct Tricker : public Player
    {
        int32_t damage;
        double trickDesire;
        double classVolume;
        TrickerType trickerType;            // 捣蛋鬼类型
        std::vector<TrickerBuffType> buff;  // buff
    };

    struct Bullet
    {
        BulletType bulletType;   // 子弹类型
        int32_t x;               // x坐标
        int32_t y;               // y坐标
        double facingDirection;  // 朝向
        int64_t guid;            // 全局唯一ID
        PlayerType team;         // 子弹所属队伍
        PlaceType place;         // 所处格子的类型
        double bombRange;        // 炸弹爆炸范围
        int32_t speed;           // 子弹速度
    };

    struct BombedBullet
    {
        BulletType bulletType;
        int32_t x;
        int32_t y;
        double facingDirection;
        int64_t mappingID;
        double bombRange;
    };

    struct Prop
    {
        int32_t x;
        int32_t y;
        int32_t size;
        int64_t guid;
        PropType type;
        PlaceType place;
        double facingDirection;  // 朝向
        bool isMoving;
    };

    struct GameMap
    {
        std::map<std::pair<int32_t, int32_t>, int32_t> classRoomState;

        std::map<std::pair<int32_t, int32_t>, int32_t> gateState;

        std::map<std::pair<int32_t, int32_t>, bool> doorState;
        std::map<std::pair<int32_t, int32_t>, int32_t> doorNumber;

        std::map<std::pair<int32_t, int32_t>, int32_t> chestState;

        std::map<std::pair<int32_t, int32_t>, bool> hiddenGateState;
    };

    struct GameInfo
    {
        int32_t gameTime;
        int32_t subjectLeft;
        int32_t studentGraduated;
        int32_t studentQuited;
        int32_t studentScore;
        int32_t trickerScore;
        bool gateOpened;
        bool hiddenGateRefreshed;
        bool hiddenGateOpened;
    };

    // 仅供DEBUG使用，名称可改动
    // 还没写完，后面待续

    inline std::map<GameState, std::string> gameStateDict{
        {GameState::NullGameState, "NullGameState"},
        {GameState::GameStart, "GameStart"},
        {GameState::GameRunning, "GameRunning"},
        {GameState::GameEnd, "GameEnd"},
    };

    inline std::map<PlayerState, std::string> playerStateDict{
        {PlayerState::NullState, "NullState"},
        {PlayerState::Idle, "Idle"},
        {PlayerState::Learning, "Learning"},
        {PlayerState::Addicted, "Addicted"},
        {PlayerState::Quit, "Quit"},
        {PlayerState::Graduated, "Graduated"},
        {PlayerState::Treated, "Treated"},
        {PlayerState::Rescued, "Rescued"},
        {PlayerState::Stunned, "Stunned"},
        {PlayerState::Treating, "Treating"},
        {PlayerState::Rescuing, "Rescuing"},
        {PlayerState::Swinging, "Swinging"},
        {PlayerState::Attacking, "Attacking"},
        {PlayerState::Locking, "Locking"},
        {PlayerState::Rummaging, "Rummaging"},
        {PlayerState::Climbing, "Climbing"},
    };

    inline std::map<PlayerType, std::string> playerTypeDict{
        {PlayerType::NullPlayerType, "NullPlayerType"},
        {PlayerType::StudentPlayer, "StudentPlayer"},
        {PlayerType::TrickerPlayer, "TrickerPlayer"},
    };

    inline std::map<PlaceType, std::string> placeTypeDict{
        {PlaceType::NullPlaceType, "NullPlaceType"},
        {PlaceType::Land, "Land"},
        {PlaceType::Wall, "Wall"},
        {PlaceType::Grass, "Grass"},
        {PlaceType::ClassRoom, "ClassRoom"},
        {PlaceType::Gate, "Gate"},
        {PlaceType::HiddenGate, "HiddenGate"},
        {PlaceType::Door, "Door"},
        {PlaceType::Window, "Window"},
        {PlaceType::Chest, "Chest"},
    };

    inline std::map<PropType, std::string> propTypeDict{
        {PropType::NullPropType, "NullPropType"},

    };

    inline std::map<StudentBuffType, std::string> studentBuffDict{
        {StudentBuffType::NullStudentBuffType, "NullStudentBuffType"},

    };

    inline std::map<TrickerBuffType, std::string> trickerBuffDict{
        {TrickerBuffType::NullTrickerBuffType, "NullTrickerBuffType"},

    };

    inline std::map<MessageOfObj, std::string> messageOfObjDict{
        {MessageOfObj::NullMessageOfObj, "NullMessageOfObj"},
        {MessageOfObj::StudentMessage, "StudentMessage"},
        {MessageOfObj::TrickerMessage, "TrickerMessage"},
        {MessageOfObj::PropMessage, "PropMessage"},
        {MessageOfObj::BulletMessage, "BulletMessage"},
        {MessageOfObj::BombedBulletMessage, "BombedBulletMessage"},
        {MessageOfObj::NullMessageOfObj, "NullMessageOfObj"},
        {MessageOfObj::ClassroomMessage, "ClassroomMessage"},
        {MessageOfObj::DoorMessage, "DoorMessage"},
        {MessageOfObj::GateMessage, "GateMessage"},
        {MessageOfObj::ChestMessage, "ChestMessage"},
        {MessageOfObj::MapMessage, "MapMessage"},
        {MessageOfObj::NewsMessage, "NewsMessage"},
        {MessageOfObj::HiddenGateMessage, "HiddenGateMessage"},
    };

}  // namespace THUAI6

#endif