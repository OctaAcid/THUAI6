﻿using System.Collections.Generic;
using GameClass.GameObj;
using System.Threading;
using Preparation.Utility;
using System;
using Timothy.FrameRateTask;
using GameEngine;
using System.Numerics;
using System.Reflection;

namespace Gaming
{
    public partial class Game
    {
        private readonly PropManager propManager;
        private class PropManager
        {
            private readonly Map gameMap;

            //private MoveEngine moveEngine;

            private readonly List<XY> availableCellForGenerateProp;

            public void UseProp(Character player, int indexing)
            {
                if (player.IsResetting)
                    return;
                Prop prop = player.UseProp(indexing);
                switch (prop.GetPropType())
                {
                    case PropType.Spear:
                        player.AddSpear(GameData.PropDuration);
                        break;
                    case PropType.Shield:
                        player.AddShield(GameData.PropDuration);
                        break;
                    case PropType.addLIFE:
                        player.AddLIFE(GameData.PropDuration);
                        break;
                    case PropType.addSpeed:
                        player.AddMoveSpeed(GameData.PropDuration);
                        break;
                    default:
                        break;
                }
            }

            /// <summary>
            ///
            /// </summary>
            /// <param name="player"></param>
            /// <param name="propType">若不指定，则自动判断可捡起什么道具</param>
            /// <returns></returns>
            public bool PickProp(Character player, PropType propType = PropType.Null)
            {
                if (player.IsResetting)
                    return false;
                int indexing = player.IndexingOfAddProp();
                if (indexing == GameData.maxNumOfPropInPropInventory)
                    return false;

                Prop pickProp = new NullProp();
                if (propType == PropType.Null)  // 自动检查有无道具可捡
                {
                    gameMap.GameObjLockDict[GameObjType.Prop].EnterReadLock();
                    try
                    {
                        foreach (Prop prop in gameMap.GameObjDict[GameObjType.Prop])
                        {
                            if (GameData.IsInTheSameCell(prop.Position, player.Position))
                            {
                                player.PropInventory[indexing] = prop;
                            }
                        }
                    }
                    finally
                    {
                        gameMap.GameObjLockDict[GameObjType.Prop].ExitReadLock();
                    }
                }
                else
                {
                    gameMap.GameObjLockDict[GameObjType.Prop].EnterReadLock();
                    try
                    {
                        foreach (Prop prop in gameMap.GameObjDict[GameObjType.Prop])
                        {
                            if (prop.GetPropType() == propType)
                            {
                                if (GameData.IsInTheSameCell(prop.Position, player.Position) && prop.CanMove == false)
                                {
                                    player.PropInventory[indexing] = prop;
                                }
                            }
                        }
                    }
                    finally
                    {
                        gameMap.GameObjLockDict[GameObjType.Prop].ExitReadLock();
                    }
                }

                if (pickProp.GetPropType() != PropType.Null)
                {
                    gameMap.Remove(pickProp);
                    gameMap.Add(new PickedProp(pickProp));
                    return true;
                }
                else
                    return false;
            }

            public void ThrowProp(Character player, int indexing)
            {
                if (!gameMap.Timer.IsGaming || player.IsResetting)
                    return;
                Prop prop = player.UseProp(indexing);
                if (prop.GetPropType() == PropType.Null)
                    return;

                prop.ReSetPos(player.Position, gameMap.GetPlaceType(player.Position));
                gameMap.Add(prop);
            }

            private Prop ProduceOnePropNotKey(Random r, XY Pos)
            {
                switch (r.Next(0, GameData.numOfPropTypeNotKey))
                {
                    case 0:
                        return new AddLIFE(Pos, gameMap.GetPlaceType(Pos));
                    case 1:
                        return new AddSpeed(Pos, gameMap.GetPlaceType(Pos));
                    case 2:
                        return new Shield(Pos, gameMap.GetPlaceType(Pos));
                    case 3:
                        return new Spear(Pos, gameMap.GetPlaceType(Pos));
                    default:
                        return null;
                }
            }

            private Chest GetChest(Random r)
            {
                int index = r.Next(0, GameData.numOfChest);
                while (((Chest)(gameMap.GameObjDict[GameObjType.Chest][index])).PropInChest[0].GetPropType() != PropType.Null) index = (index + 1) % GameData.numOfChest;
                return (Chest)(gameMap.GameObjDict[GameObjType.Chest][index]);
            }

            public void StartProducing()
            {
                int len = availableCellForGenerateProp.Count;
                Random r = new Random(Environment.TickCount);

                gameMap.GameObjLockDict[GameObjType.Chest].EnterWriteLock();
                try
                {
                    int cou = 0;
                    while (cou < GameData.numOfKeyEachArea)
                    {
                        ++cou;
                        Chest chest = GetChest(r);
                        chest.PropInChest[1] = new Key3(chest.Position, PlaceType.Chest);
                        chest.PropInChest[0] = ProduceOnePropNotKey(r, chest.Position);
                    }
                    cou = 0;
                    while (cou < GameData.numOfKeyEachArea)
                    {
                        ++cou;
                        Chest chest = GetChest(r);
                        chest.PropInChest[1] = new Key5(chest.Position, PlaceType.Chest);
                        chest.PropInChest[0] = ProduceOnePropNotKey(r, chest.Position);
                    }
                    cou = 0;
                    while (cou < GameData.numOfKeyEachArea)
                    {
                        ++cou;
                        Chest chest = GetChest(r);
                        chest.PropInChest[1] = new Key6(chest.Position, PlaceType.Chest);
                        chest.PropInChest[0] = ProduceOnePropNotKey(r, chest.Position);
                    }

                    foreach (Chest chest in gameMap.GameObjDict[GameObjType.Chest])
                    {
                        if (chest.PropInChest[0].GetPropType() == PropType.Null)
                        {
                            chest.PropInChest[0] = ProduceOnePropNotKey(r, chest.Position);
                            chest.PropInChest[1] = ProduceOnePropNotKey(r, chest.Position);
                        }
                    }
                }
                finally
                {
                    gameMap.GameObjLockDict[GameObjType.Chest].ExitWriteLock();
                }

                new Thread
                (
                    () =>
                    {
                        while (!gameMap.Timer.IsGaming)
                            Thread.Sleep(1000);
                        new FrameRateTaskExecutor<int>(
                            () => gameMap.Timer.IsGaming,
                            () =>
                            {
                                int rand = r.Next(0, len);
                                XY randPos = availableCellForGenerateProp[rand];
                                gameMap.Add(ProduceOnePropNotKey(r, randPos));
                            },
                            GameData.PropProduceTime,
                            () => 0
                        )
                            .Start();
                    }
                )
                { IsBackground = true }.Start();
            }
            public PropManager(Map gameMap)  // 道具不能扔过墙
            {
                this.gameMap = gameMap;
                /*            this.moveEngine = new MoveEngine(
                                gameMap: gameMap,
                                OnCollision: (obj, collision, moveVec) =>
                                { return MoveEngine.AfterCollision.MoveMax; },
                                EndMove: obj =>
                                {
                                    // obj.Place = gameMap.GetPlaceType((GameObj)obj);
                                    obj.CanMove = false;
                                    Debugger.Output(obj, " end move at " + obj.Position.ToString() + " At time: " + Environment.TickCount64);
                                }
                            );*/
                availableCellForGenerateProp = new List<XY>();
                for (int i = 0; i < gameMap.protoGameMap.GetLength(0); i++)
                {
                    for (int j = 0; j < gameMap.protoGameMap.GetLength(1); j++)
                    {
                        if (gameMap.protoGameMap[i, j] == (int)PlaceType.Null)
                        {
                            availableCellForGenerateProp.Add(GameData.GetCellCenterPos(i, j));
                        }
                    }
                }
            }
        }
    }
}