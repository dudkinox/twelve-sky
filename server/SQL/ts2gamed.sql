/*
 Navicat Premium Data Transfer

 Source Server         : 12Sky2
 Source Server Type    : MySQL
 Source Server Version : 50729
 Source Host           : 194.31.64.123:3306
 Source Schema         : ts2_gamedb

 Target Server Type    : MySQL
 Target Server Version : 50729
 File Encoding         : 65001

 Date: 20/04/2020 10:43:37
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for AllAvatarInfo
-- ----------------------------
DROP TABLE IF EXISTS `AllAvatarInfo`;
CREATE TABLE `AllAvatarInfo`  (
  `LogDate` datetime(0) NULL DEFAULT NULL,
  `World` varchar(6) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uID` varchar(25) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aName` varchar(25) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aTribe` smallint(6) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of AllAvatarInfo
-- ----------------------------

-- ----------------------------
-- Table structure for AutoUserList
-- ----------------------------
DROP TABLE IF EXISTS `AutoUserList`;
CREATE TABLE `AutoUserList`  (
  `aDate` datetime(0) NULL DEFAULT NULL,
  `tID` int(11) NULL DEFAULT NULL,
  `tAvatarName` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `tWorldNumber` varchar(14) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `tZoneNumber` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of AutoUserList
-- ----------------------------

-- ----------------------------
-- Table structure for AvatarInfo
-- ----------------------------
DROP TABLE IF EXISTS `AvatarInfo`;
CREATE TABLE `AvatarInfo`  (
  `aCreate` datetime(0) NULL DEFAULT NULL,
  `uID` varchar(12) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aVisibleState` int(11) NULL DEFAULT 0,
  `aSpecialState` int(11) NULL DEFAULT 0,
  `aPlayTime1` int(11) NULL DEFAULT 0,
  `aPlayTime2` int(11) NULL DEFAULT 0,
  `aPlayTime3` int(11) NULL DEFAULT 0,
  `aKillOtherTribe` int(11) NULL DEFAULT 0,
  `aWorldNumber` int(11) NULL DEFAULT 0,
  `aName` varchar(24) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aTribe` int(11) NULL DEFAULT 0,
  `aPreviousTribe` int(11) NULL DEFAULT 0,
  `aGender` int(11) NULL DEFAULT 0,
  `aHeadType` int(11) NULL DEFAULT 0,
  `aFaceType` int(11) NULL DEFAULT 0,
  `aLevel1` int(11) NULL DEFAULT 0,
  `aLevel2` int(11) NULL DEFAULT 0,
  `aGeneralExperience1` int(11) NULL DEFAULT 0,
  `aGeneralExperience2` int(11) NULL DEFAULT 0,
  `aVitality` int(11) NULL DEFAULT 0,
  `aStrength` int(11) NULL DEFAULT 0,
  `aKi` int(11) NULL DEFAULT 0,
  `aWisdom` int(11) NULL DEFAULT 0,
  `aEatLifePotion` int(11) NULL DEFAULT 0,
  `aEatManaPotion` int(11) NULL DEFAULT 0,
  `aStateBonusPoint` int(11) NULL DEFAULT 0,
  `aSkillPoint` int(11) NULL DEFAULT NULL,
  `aEquip` varbinary(5000) NULL DEFAULT NULL,
  `aExpandInventoryDate` int(11) NULL DEFAULT NULL,
  `aMoney` int(11) NULL DEFAULT 0,
  `aInventory` varbinary(5000) NULL DEFAULT NULL,
  `aTradeMoney` int(11) NULL DEFAULT 0,
  `aTrade` varbinary(5000) NULL DEFAULT NULL,
  `aExpandStoreDate` int(11) NULL DEFAULT NULL,
  `aStoreMoney` int(11) NULL DEFAULT 0,
  `aStoreItem` varbinary(5000) NULL DEFAULT NULL,
  `aSkill` varbinary(5000) NULL DEFAULT NULL,
  `aHotKey` varbinary(5000) NULL DEFAULT NULL,
  `aQuestInfo` varbinary(5000) NULL DEFAULT NULL,
  `aFriend` varbinary(5000) NULL DEFAULT NULL,
  `aTeacher` varbinary(5000) NULL DEFAULT NULL,
  `aStudent` varbinary(5000) NULL DEFAULT NULL,
  `aTeacherPoint` int(11) NULL DEFAULT NULL,
  `aGuildName` varbinary(5000) NULL DEFAULT NULL,
  `aGuildRole` int(11) NULL DEFAULT NULL,
  `aCallName` varbinary(5000) NULL DEFAULT NULL,
  `aGuildMarkNum` int(11) NULL DEFAULT NULL,
  `aGuildMarkEffect` int(11) NULL DEFAULT NULL,
  `aLogoutInfo01` int(11) NULL DEFAULT NULL,
  `aLogoutInfo02` int(11) NULL DEFAULT NULL,
  `aLogoutInfo03` int(11) NULL DEFAULT NULL,
  `aLogoutInfo04` int(11) NULL DEFAULT NULL,
  `aLogoutInfo05` int(11) NULL DEFAULT NULL,
  `aLogoutInfo06` int(11) NULL DEFAULT NULL,
  `aProtectForDeath` int(11) NULL DEFAULT NULL,
  `aProtectForDestroy` int(11) NULL DEFAULT NULL,
  `aDoubleExpTime1` int(11) NULL DEFAULT NULL,
  `aDoubleExpTime2` int(11) NULL DEFAULT NULL,
  `aDoubleKillNumTime` int(11) NULL DEFAULT NULL,
  `aDoubleKillExpTime` int(11) NULL DEFAULT NULL,
  `aNineTowerKeyNum` int(11) NULL DEFAULT NULL,
  `aZone101TypeTime` int(11) NULL DEFAULT NULL,
  `aZone125TypeTime` int(11) NULL DEFAULT NULL,
  `aZone126TypeTime` int(11) NULL DEFAULT NULL,
  `aKillMonsterNum` int(11) NULL DEFAULT NULL,
  `aLevelZoneKeyNum` int(11) NULL DEFAULT NULL,
  `aSearchAndBuyDate` int(11) NULL DEFAULT NULL,
  `aLifePotionConvertNum` int(11) NULL DEFAULT NULL,
  `aManaPotionConvertNum` int(11) NULL DEFAULT NULL,
  `aTribeVoteDate` int(11) NULL DEFAULT NULL,
  `aUse_AutoPotion_Date` int(11) NULL DEFAULT NULL,
  `aUse_AutoPotion_Time` int(11) NULL DEFAULT NULL,
  `aAutoLifeRatio` int(11) NULL DEFAULT NULL,
  `aAutoManaRatio` int(11) NULL DEFAULT NULL,
  `aAnimal01` int(11) NULL DEFAULT NULL,
  `aAnimal02` int(11) NULL DEFAULT NULL,
  `aAnimal03` int(11) NULL DEFAULT NULL,
  `aAnimal04` int(11) NULL DEFAULT NULL,
  `aAnimal05` int(11) NULL DEFAULT NULL,
  `aAnimal06` int(11) NULL DEFAULT NULL,
  `aAnimal07` int(11) NULL DEFAULT NULL,
  `aAnimal08` int(11) NULL DEFAULT NULL,
  `aAnimal09` int(11) NULL DEFAULT NULL,
  `aAnimal10` int(11) NULL DEFAULT NULL,
  `aAnimalIndex` int(11) NULL DEFAULT NULL,
  `aAnimalTime` int(11) NULL DEFAULT NULL,
  `aDoubleExpTime3` int(11) NULL DEFAULT NULL,
  `aDoubleDropTime` int(11) NULL DEFAULT NULL,
  `iCriminal_Record` int(11) NULL DEFAULT NULL,
  `iEnter_The_Prison` int(11) NULL DEFAULT NULL,
  `iKill_Monster_Count` int(11) NULL DEFAULT NULL,
  `aEatStrengthPotion` int(11) NULL DEFAULT NULL,
  `aEatWisdomPotion` int(11) NULL DEFAULT NULL,
  `aPat_Exp_x2_Time` int(11) NULL DEFAULT NULL,
  `aPat_User_Exp_x2_Time` int(11) NULL DEFAULT NULL,
  `aFiveExpTime` int(11) NULL DEFAULT NULL,
  `aZone84Time` int(11) NULL DEFAULT NULL,
  `iSilverOrnamentRemainsTime` int(11) NULL DEFAULT NULL,
  `iGoldOrnamentRemainsTime` int(11) NULL DEFAULT NULL,
  `iUseOrnament` int(11) NULL DEFAULT NULL,
  `aLevelZoneKeyNum2` int(11) NULL DEFAULT NULL,
  `aDoubleKillNumTime2` int(11) NULL DEFAULT NULL,
  `aKillMonsterNum2` int(11) NULL DEFAULT NULL,
  `aAddItemValue` int(11) NULL DEFAULT NULL,
  `aHighItemValue` int(11) NULL DEFAULT NULL,
  `aDropItemTime` int(11) NULL DEFAULT NULL,
  `aTribeCallValue` int(11) NULL DEFAULT NULL,
  `aUpgradeValue` int(11) NULL DEFAULT NULL,
  `aBonusItemValue` int(11) NULL DEFAULT NULL,
  `aKillOtherTribeEvent` int(11) NULL DEFAULT NULL,
  `aTeacherPointEvent` int(11) NULL DEFAULT NULL,
  `aPlayTimeEvent` int(11) NULL DEFAULT NULL,
  `aProtectForUpgrade` int(11) NULL DEFAULT NULL,
  UNIQUE INDEX `aName`(`aName`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of AvatarInfo
-- ----------------------------

-- ----------------------------
-- Table structure for AvatarInfoCreateLog
-- ----------------------------
DROP TABLE IF EXISTS `AvatarInfoCreateLog`;
CREATE TABLE `AvatarInfoCreateLog`  (
  `LogDate` datetime(0) NULL DEFAULT NULL,
  `RCount` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of AvatarInfoCreateLog
-- ----------------------------

-- ----------------------------
-- Table structure for AvatarNameInfo
-- ----------------------------
DROP TABLE IF EXISTS `AvatarNameInfo`;
CREATE TABLE `AvatarNameInfo`  (
  `aDate` datetime(0) NULL DEFAULT NULL,
  `aName` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of AvatarNameInfo
-- ----------------------------

-- ----------------------------
-- Table structure for BadUserInfo
-- ----------------------------
DROP TABLE IF EXISTS `BadUserInfo`;
CREATE TABLE `BadUserInfo`  (
  `bDate` datetime(0) NULL DEFAULT NULL,
  `uID` varchar(24) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `uWarningNum` int(11) NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of BadUserInfo
-- ----------------------------

-- ----------------------------
-- Table structure for BadUserList
-- ----------------------------
DROP TABLE IF EXISTS `BadUserList`;
CREATE TABLE `BadUserList`  (
  `bDate` datetime(0) NULL DEFAULT NULL,
  `tID` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `tSort` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `tHackName` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of BadUserList
-- ----------------------------

-- ----------------------------
-- Table structure for BonusInfo
-- ----------------------------
DROP TABLE IF EXISTS `BonusInfo`;
CREATE TABLE `BonusInfo`  (
  `uID` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aName` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aLevel` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of BonusInfo
-- ----------------------------

-- ----------------------------
-- Table structure for DefineMaxUserNum
-- ----------------------------
DROP TABLE IF EXISTS `DefineMaxUserNum`;
CREATE TABLE `DefineMaxUserNum`  (
  `mMaxUserNum` int(11) NULL DEFAULT NULL,
  `mAddPlayerNum` int(11) NULL DEFAULT NULL,
  `mGageUserNum` int(11) NULL DEFAULT NULL,
  `mAddUserNum` int(11) NULL DEFAULT NULL,
  `aWorldNumber` int(11) NULL DEFAULT NULL
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of DefineMaxUserNum
-- ----------------------------
INSERT INTO `DefineMaxUserNum` VALUES (1000, 50, 50, 50, 1);

-- ----------------------------
-- Table structure for DeleteAvatarInfo
-- ----------------------------
DROP TABLE IF EXISTS `DeleteAvatarInfo`;
CREATE TABLE `DeleteAvatarInfo`  (
  `aCreate` datetime(0) NULL DEFAULT NULL,
  `uID` varchar(12) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aVisibleState` int(11) NULL DEFAULT 0,
  `aSpecialState` int(11) NULL DEFAULT 0,
  `aPlayTime1` int(11) NULL DEFAULT 0,
  `aPlayTime2` int(11) NULL DEFAULT 0,
  `aPlayTime3` int(11) NULL DEFAULT 0,
  `aKillOtherTribe` int(11) NULL DEFAULT 0,
  `aWorldNumber` int(11) NULL DEFAULT 0,
  `aName` varchar(24) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aTribe` int(11) NULL DEFAULT 0,
  `aPreviousTribe` int(11) NULL DEFAULT 0,
  `aGender` int(11) NULL DEFAULT 0,
  `aHeadType` int(11) NULL DEFAULT 0,
  `aFaceType` int(11) NULL DEFAULT 0,
  `aLevel1` int(11) NULL DEFAULT 0,
  `aLevel2` int(11) NULL DEFAULT 0,
  `aGeneralExperience1` int(11) NULL DEFAULT 0,
  `aGeneralExperience2` int(11) NULL DEFAULT 0,
  `aVitality` int(11) NULL DEFAULT 0,
  `aStrength` int(11) NULL DEFAULT 0,
  `aKi` int(11) NULL DEFAULT 0,
  `aWisdom` int(11) NULL DEFAULT 0,
  `aEatLifePotion` int(11) NULL DEFAULT 0,
  `aEatManaPotion` int(11) NULL DEFAULT 0,
  `aStateBonusPoint` int(11) NULL DEFAULT 0,
  `aSkillPoint` int(11) NULL DEFAULT NULL,
  `aEquip` varbinary(5000) NULL DEFAULT NULL,
  `aExpandInventoryDate` int(11) NULL DEFAULT NULL,
  `aMoney` int(11) NULL DEFAULT 0,
  `aInventory` varbinary(5000) NULL DEFAULT NULL,
  `aTradeMoney` int(11) NULL DEFAULT 0,
  `aTrade` varbinary(5000) NULL DEFAULT NULL,
  `aExpandStoreDate` int(11) NULL DEFAULT NULL,
  `aStoreMoney` int(11) NULL DEFAULT 0,
  `aStoreItem` varbinary(5000) NULL DEFAULT NULL,
  `aSkill` varbinary(5000) NULL DEFAULT NULL,
  `aHotKey` varbinary(5000) NULL DEFAULT NULL,
  `aQuestInfo` varbinary(5000) NULL DEFAULT NULL,
  `aFriend` varbinary(5000) NULL DEFAULT NULL,
  `aTeacher` varbinary(5000) NULL DEFAULT NULL,
  `aStudent` varbinary(5000) NULL DEFAULT NULL,
  `aTeacherPoint` int(11) NULL DEFAULT NULL,
  `aGuildName` varbinary(5000) NULL DEFAULT NULL,
  `aGuildRole` int(11) NULL DEFAULT NULL,
  `aCallName` varbinary(5000) NULL DEFAULT NULL,
  `aGuildMarkNum` int(11) NULL DEFAULT NULL,
  `aGuildMarkEffect` int(11) NULL DEFAULT NULL,
  `aLogoutInfo01` int(11) NULL DEFAULT NULL,
  `aLogoutInfo02` int(11) NULL DEFAULT NULL,
  `aLogoutInfo03` int(11) NULL DEFAULT NULL,
  `aLogoutInfo04` int(11) NULL DEFAULT NULL,
  `aLogoutInfo05` int(11) NULL DEFAULT NULL,
  `aLogoutInfo06` int(11) NULL DEFAULT NULL,
  `aProtectForDeath` int(11) NULL DEFAULT NULL,
  `aProtectForDestroy` int(11) NULL DEFAULT NULL,
  `aDoubleExpTime1` int(11) NULL DEFAULT NULL,
  `aDoubleExpTime2` int(11) NULL DEFAULT NULL,
  `aDoubleKillNumTime` int(11) NULL DEFAULT NULL,
  `aDoubleKillExpTime` int(11) NULL DEFAULT NULL,
  `aNineTowerKeyNum` int(11) NULL DEFAULT NULL,
  `aZone101TypeTime` int(11) NULL DEFAULT NULL,
  `aZone125TypeTime` int(11) NULL DEFAULT NULL,
  `aZone126TypeTime` int(11) NULL DEFAULT NULL,
  `aKillMonsterNum` int(11) NULL DEFAULT NULL,
  `aLevelZoneKeyNum` int(11) NULL DEFAULT NULL,
  `aSearchAndBuyDate` int(11) NULL DEFAULT NULL,
  `aLifePotionConvertNum` int(11) NULL DEFAULT NULL,
  `aManaPotionConvertNum` int(11) NULL DEFAULT NULL,
  `aTribeVoteDate` int(11) NULL DEFAULT NULL,
  `aUse_AutoPotion_Date` int(11) NULL DEFAULT NULL,
  `aUse_AutoPotion_Time` int(11) NULL DEFAULT NULL,
  `aAutoLifeRatio` int(11) NULL DEFAULT NULL,
  `aAutoManaRatio` int(11) NULL DEFAULT NULL,
  `aAnimal01` int(11) NULL DEFAULT NULL,
  `aAnimal02` int(11) NULL DEFAULT NULL,
  `aAnimal03` int(11) NULL DEFAULT NULL,
  `aAnimal04` int(11) NULL DEFAULT NULL,
  `aAnimal05` int(11) NULL DEFAULT NULL,
  `aAnimal06` int(11) NULL DEFAULT NULL,
  `aAnimal07` int(11) NULL DEFAULT NULL,
  `aAnimal08` int(11) NULL DEFAULT NULL,
  `aAnimal09` int(11) NULL DEFAULT NULL,
  `aAnimal10` int(11) NULL DEFAULT NULL,
  `aAnimalIndex` int(11) NULL DEFAULT NULL,
  `aAnimalTime` int(11) NULL DEFAULT NULL,
  `aDoubleExpTime3` int(11) NULL DEFAULT NULL,
  `aDoubleDropTime` int(11) NULL DEFAULT NULL,
  `iCriminal_Record` int(11) NULL DEFAULT NULL,
  `iEnter_The_Prison` int(11) NULL DEFAULT NULL,
  `iKill_Monster_Count` int(11) NULL DEFAULT NULL,
  `aEatStrengthPotion` int(11) NULL DEFAULT NULL,
  `aEatWisdomPotion` int(11) NULL DEFAULT NULL,
  `aPat_Exp_x2_Time` int(11) NULL DEFAULT NULL,
  `aPat_User_Exp_x2_Time` int(11) NULL DEFAULT NULL,
  `aFiveExpTime` int(11) NULL DEFAULT NULL,
  `aZone84Time` int(11) NULL DEFAULT NULL,
  `iSilverOrnamentRemainsTime` int(11) NULL DEFAULT NULL,
  `iGoldOrnamentRemainsTime` int(11) NULL DEFAULT NULL,
  `iUseOrnament` int(11) NULL DEFAULT NULL,
  `aLevelZoneKeyNum2` int(11) NULL DEFAULT NULL,
  `aDoubleKillNumTime2` int(11) NULL DEFAULT NULL,
  `aKillMonsterNum2` int(11) NULL DEFAULT NULL,
  `aAddItemValue` int(11) NULL DEFAULT NULL,
  `aHighItemValue` int(11) NULL DEFAULT NULL,
  `aDropItemTime` int(11) NULL DEFAULT NULL,
  `aTribeCallValue` int(11) NULL DEFAULT NULL,
  `aUpgradeValue` int(11) NULL DEFAULT NULL,
  `aBonusItemValue` int(11) NULL DEFAULT NULL,
  `aKillOtherTribeEvent` int(11) NULL DEFAULT NULL,
  `aTeacherPointEvent` int(11) NULL DEFAULT NULL,
  `aPlayTimeEvent` int(11) NULL DEFAULT NULL,
  `aProtectForUpgrade` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of DeleteAvatarInfo
-- ----------------------------

-- ----------------------------
-- Table structure for dtproperties
-- ----------------------------
DROP TABLE IF EXISTS `dtproperties`;
CREATE TABLE `dtproperties`  (
  `id` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `objectid` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `property` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `value` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uvalue` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `lvalue` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `version` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of dtproperties
-- ----------------------------

-- ----------------------------
-- Table structure for event_id_list
-- ----------------------------
DROP TABLE IF EXISTS `event_id_list`;
CREATE TABLE `event_id_list`  (
  `uid` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `eventid` float(10, 5) NULL DEFAULT NULL,
  `aname` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `serverid` float(10, 5) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of event_id_list
-- ----------------------------

-- ----------------------------
-- Table structure for EventInfo
-- ----------------------------
DROP TABLE IF EXISTS `EventInfo`;
CREATE TABLE `EventInfo`  (
  `uID` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uEventNum01` int(11) NULL DEFAULT 0,
  `uEventNum02` int(11) NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of EventInfo
-- ----------------------------
INSERT INTO `EventInfo` VALUES ('davyjones', 1, 0);

-- ----------------------------
-- Table structure for GiftInfo
-- ----------------------------
DROP TABLE IF EXISTS `GiftInfo`;
CREATE TABLE `GiftInfo`  (
  `uID` int(11) NOT NULL,
  `gData1` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `gNumber1` int(11) NULL DEFAULT NULL,
  `gData2` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `gNumber2` int(11) NULL DEFAULT NULL,
  `gData3` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `gNumber3` int(11) NULL DEFAULT NULL,
  `gData4` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `gNumber4` int(11) NULL DEFAULT NULL,
  `gData5` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `gNumber5` int(11) NULL DEFAULT NULL,
  `gCard` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  PRIMARY KEY (`uID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of GiftInfo
-- ----------------------------

-- ----------------------------
-- Table structure for GuildEventHelper
-- ----------------------------
DROP TABLE IF EXISTS `GuildEventHelper`;
CREATE TABLE `GuildEventHelper`  (
  `startDate` datetime(0) NULL DEFAULT NULL,
  `endDate` datetime(0) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of GuildEventHelper
-- ----------------------------
INSERT INTO `GuildEventHelper` VALUES (NULL, NULL, 0);

-- ----------------------------
-- Table structure for GuildEventInfo
-- ----------------------------
DROP TABLE IF EXISTS `GuildEventInfo`;
CREATE TABLE `GuildEventInfo`  (
  `gName` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `gScore` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of GuildEventInfo
-- ----------------------------

-- ----------------------------
-- Table structure for GuildInfo
-- ----------------------------
DROP TABLE IF EXISTS `GuildInfo`;
CREATE TABLE `GuildInfo`  (
  `iDate` datetime(0) NULL DEFAULT NULL,
  `aWorldNumber` int(11) NULL DEFAULT NULL,
  `gName` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `gGrade` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gMaster01` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gSubMaster01` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gSubMaster02` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gMember` text CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL,
  `gNotice01` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gNotice02` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gNotice03` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gNotice04` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of GuildInfo
-- ----------------------------

-- ----------------------------
-- Table structure for HackInfo
-- ----------------------------
DROP TABLE IF EXISTS `HackInfo`;
CREATE TABLE `HackInfo`  (
  `hSize` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of HackInfo
-- ----------------------------

-- ----------------------------
-- Table structure for inuser
-- ----------------------------
DROP TABLE IF EXISTS `inuser`;
CREATE TABLE `inuser`  (
  `uid` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of inuser
-- ----------------------------

-- ----------------------------
-- Table structure for jhevent
-- ----------------------------
DROP TABLE IF EXISTS `jhevent`;
CREATE TABLE `jhevent`  (
  `eventid` int(11) NULL DEFAULT 0,
  `subeventid` int(11) NULL DEFAULT 0,
  `itemid` varchar(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `itemname` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `itemnum` int(11) NULL DEFAULT 0,
  `itemclass` int(11) NULL DEFAULT 0,
  `IsMuti` int(11) NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of jhevent
-- ----------------------------

-- ----------------------------
-- Table structure for jheventlog
-- ----------------------------
DROP TABLE IF EXISTS `jheventlog`;
CREATE TABLE `jheventlog`  (
  `uid` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `sendtime` datetime(0) NULL DEFAULT NULL,
  `eventid` int(11) NULL DEFAULT 0,
  `subeventid` int(11) NULL DEFAULT 0,
  `sendflag` int(11) NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of jheventlog
-- ----------------------------

-- ----------------------------
-- Table structure for jheventuser
-- ----------------------------
DROP TABLE IF EXISTS `jheventuser`;
CREATE TABLE `jheventuser`  (
  `uid` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `eventid` int(11) NULL DEFAULT 0,
  `sendflag` int(11) NULL DEFAULT 0
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of jheventuser
-- ----------------------------

-- ----------------------------
-- Table structure for LevelDistribution
-- ----------------------------
DROP TABLE IF EXISTS `LevelDistribution`;
CREATE TABLE `LevelDistribution`  (
  `LogDate` datetime(0) NULL DEFAULT NULL,
  `GroupType` smallint(6) NULL DEFAULT 0,
  `aLev` int(11) NULL DEFAULT NULL,
  `bLev` int(11) NULL DEFAULT NULL,
  `cLev` int(11) NULL DEFAULT NULL,
  `dLev` int(11) NULL DEFAULT NULL,
  `eLev` int(11) NULL DEFAULT NULL,
  `fLev` int(11) NULL DEFAULT NULL,
  `gLev` int(11) NULL DEFAULT NULL,
  `hLev` int(11) NULL DEFAULT NULL,
  `iLev` int(11) NULL DEFAULT NULL,
  `jLev` int(11) NULL DEFAULT NULL,
  `kLev` int(11) NULL DEFAULT NULL,
  `lLev` int(11) NULL DEFAULT NULL,
  `mLev` int(11) NULL DEFAULT NULL,
  `nLev` int(11) NULL DEFAULT NULL,
  `tCount` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of LevelDistribution
-- ----------------------------

-- ----------------------------
-- Table structure for LevelDistribution_detail
-- ----------------------------
DROP TABLE IF EXISTS `LevelDistribution_detail`;
CREATE TABLE `LevelDistribution_detail`  (
  `LogDate` datetime(0) NULL DEFAULT NULL,
  `GroupType` smallint(6) NULL DEFAULT NULL,
  `[1]` int(11) NULL DEFAULT NULL,
  `[10]` int(11) NULL DEFAULT NULL,
  `[20]` int(11) NULL DEFAULT NULL,
  `[30]` int(11) NULL DEFAULT NULL,
  `[40]` int(11) NULL DEFAULT NULL,
  `[50]` int(11) NULL DEFAULT NULL,
  `[60]` int(11) NULL DEFAULT NULL,
  `[70]` int(11) NULL DEFAULT NULL,
  `[80]` int(11) NULL DEFAULT NULL,
  `[90]` int(11) NULL DEFAULT NULL,
  `[100]` int(11) NULL DEFAULT NULL,
  `[110]` int(11) NULL DEFAULT NULL,
  `[111]` int(11) NULL DEFAULT NULL,
  `[112]` int(11) NULL DEFAULT NULL,
  `[113]` int(11) NULL DEFAULT NULL,
  `[114]` int(11) NULL DEFAULT NULL,
  `[115]` int(11) NULL DEFAULT NULL,
  `[116]` int(11) NULL DEFAULT NULL,
  `[117]` int(11) NULL DEFAULT NULL,
  `[118]` int(11) NULL DEFAULT NULL,
  `[119]` int(11) NULL DEFAULT NULL,
  `[120]` int(11) NULL DEFAULT NULL,
  `[121]` int(11) NULL DEFAULT NULL,
  `[122]` int(11) NULL DEFAULT NULL,
  `[123]` int(11) NULL DEFAULT NULL,
  `[124]` int(11) NULL DEFAULT NULL,
  `[125]` int(11) NULL DEFAULT NULL,
  `[126]` int(11) NULL DEFAULT NULL,
  `[127]` int(11) NULL DEFAULT NULL,
  `[128]` int(11) NULL DEFAULT NULL,
  `[129]` int(11) NULL DEFAULT NULL,
  `[130]` int(11) NULL DEFAULT NULL,
  `[131]` int(11) NULL DEFAULT NULL,
  `[132]` int(11) NULL DEFAULT NULL,
  `[133]` int(11) NULL DEFAULT NULL,
  `[134]` int(11) NULL DEFAULT NULL,
  `[135]` int(11) NULL DEFAULT NULL,
  `[136]` int(11) NULL DEFAULT NULL,
  `[137]` int(11) NULL DEFAULT NULL,
  `[138]` int(11) NULL DEFAULT NULL,
  `[139]` int(11) NULL DEFAULT NULL,
  `[140]` int(11) NULL DEFAULT NULL,
  `[141]` int(11) NULL DEFAULT NULL,
  `[142]` int(11) NULL DEFAULT NULL,
  `[143]` int(11) NULL DEFAULT NULL,
  `[144]` int(11) NULL DEFAULT NULL,
  `[145]` int(11) NULL DEFAULT NULL,
  `tCount` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of LevelDistribution_detail
-- ----------------------------

-- ----------------------------
-- Table structure for LoginInfo
-- ----------------------------
DROP TABLE IF EXISTS `LoginInfo`;
CREATE TABLE `LoginInfo`  (
  `uID` varchar(6) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uPassword` varchar(8) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uPermit` smallint(6) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of LoginInfo
-- ----------------------------

-- ----------------------------
-- Table structure for loginUser
-- ----------------------------
DROP TABLE IF EXISTS `loginUser`;
CREATE TABLE `loginUser`  (
  `uID` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aName` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `nowlevel` int(11) NULL DEFAULT NULL,
  `backlevel` int(11) NULL DEFAULT NULL,
  `gaplevel` int(11) NULL DEFAULT NULL,
  `nowexp` int(11) NULL DEFAULT NULL,
  `backexp` int(11) NULL DEFAULT NULL,
  `gapexp` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of loginUser
-- ----------------------------

-- ----------------------------
-- Table structure for LoginVersionInfo
-- ----------------------------
DROP TABLE IF EXISTS `LoginVersionInfo`;
CREATE TABLE `LoginVersionInfo`  (
  `LoginVersion` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `LoginVersionSeq` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of LoginVersionInfo
-- ----------------------------

-- ----------------------------
-- Table structure for MasterInfo
-- ----------------------------
DROP TABLE IF EXISTS `MasterInfo`;
CREATE TABLE `MasterInfo`  (
  `uLogintime` datetime(0) NULL DEFAULT NULL,
  `aWorldNumber` tinyint(4) NULL DEFAULT NULL,
  `uID` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uSaveMoney` int(11) NULL DEFAULT NULL,
  `uSaveItem` varchar(700) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aName01` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aName02` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aName03` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of MasterInfo
-- ----------------------------

-- ----------------------------
-- Table structure for MemberAuth
-- ----------------------------
DROP TABLE IF EXISTS `MemberAuth`;
CREATE TABLE `MemberAuth`  (
  `uID` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `AuthType` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `FindFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `MoveFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `MoveZoneFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `CallFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `HideFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `ShowFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `KickFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `BlockFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `MonCallFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `EquipFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `UnequipFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `YchatFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `NchatFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `NoticeFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `ItemFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `MoneyFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `ExpFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `DieFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `GodFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `ChatFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `TradeFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `ShopFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `PShopFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `GItemFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `DItemFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `AttackFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `UZonemoveFlag` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  UNIQUE INDEX `uID`(`uID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of MemberAuth
-- ----------------------------

-- ----------------------------
-- Table structure for MemberInfo
-- ----------------------------
DROP TABLE IF EXISTS `MemberInfo`;
CREATE TABLE `MemberInfo`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `uID` varchar(50) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `uPassword` varchar(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT '',
  `uPassword_temp` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT NULL,
  `aFullname` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT NULL,
  `uMail` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT NULL,
  `uMousePassword` varchar(4) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT '',
  `uBlockInfo` int(11) NULL DEFAULT 0,
  `uUserSort` int(1) NULL DEFAULT 0,
  `uTraceState` int(1) NULL DEFAULT 0,
  `uCash` int(11) NULL DEFAULT 0,
  `uLoginState` int(11) NULL DEFAULT 0,
  `uIP` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `uCheckMOTP` int(11) NULL DEFAULT 0,
  `uSecretCard` int(11) NULL DEFAULT 0,
  `uGoodFellow` int(11) NULL DEFAULT 0,
  `uLoginPlace` int(11) NULL DEFAULT 0,
  `uLoginPremium` int(11) NULL DEFAULT 0,
  `uBonus100Money` char(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT '0',
  `uPremiumServerExpirationDate` int(11) NULL DEFAULT 0,
  `uUpdateTime` datetime(0) NULL DEFAULT NULL,
  `aWorldNumber` int(11) NULL DEFAULT 1,
  `uBonusCash` int(11) NULL DEFAULT 0,
  `uVerify` int(11) NULL DEFAULT 0,
  `uMailHash` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT NULL,
  `uWheelLevel` int(11) NULL DEFAULT 0,
  `uWheelStep` int(11) NULL DEFAULT 0,
  `uWheelitem` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `uID`(`uID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of MemberInfo
-- ----------------------------

-- ----------------------------
-- Table structure for PaymentForm
-- ----------------------------
DROP TABLE IF EXISTS `PaymentForm`;
CREATE TABLE `PaymentForm`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `uID` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `paymentReference` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT 0,
  `totalprice` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `cash` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `pIP` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `pTime` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of PaymentForm
-- ----------------------------

-- ----------------------------
-- Table structure for PC_Info
-- ----------------------------
DROP TABLE IF EXISTS `PC_Info`;
CREATE TABLE `PC_Info`  (
  `mPCIP` varchar(15) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of PC_Info
-- ----------------------------

-- ----------------------------
-- Table structure for PcRoomIpInfo
-- ----------------------------
DROP TABLE IF EXISTS `PcRoomIpInfo`;
CREATE TABLE `PcRoomIpInfo`  (
  `uStartIP` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `uEndIP` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of PcRoomIpInfo
-- ----------------------------
INSERT INTO `PcRoomIpInfo` VALUES ('104.248.24.29', '104.248.24.29');
INSERT INTO `PcRoomIpInfo` VALUES ('31.223.116.127', '31.223.116.127');

-- ----------------------------
-- Table structure for playtimenewda
-- ----------------------------
DROP TABLE IF EXISTS `playtimenewda`;
CREATE TABLE `playtimenewda`  (
  `uid` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aname` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `F3` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `F4` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `F5` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of playtimenewda
-- ----------------------------

-- ----------------------------
-- Table structure for playtimenewxiao
-- ----------------------------
DROP TABLE IF EXISTS `playtimenewxiao`;
CREATE TABLE `playtimenewxiao`  (
  `uid` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aname` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `F3` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `F4` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `F5` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of playtimenewxiao
-- ----------------------------

-- ----------------------------
-- Table structure for RankInfo
-- ----------------------------
DROP TABLE IF EXISTS `RankInfo`;
CREATE TABLE `RankInfo`  (
  `uUpdateTime` datetime(0) NULL DEFAULT NULL,
  `aUpdateTime` datetime(0) NULL DEFAULT NULL,
  `uID` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aWorldNumber` int(11) NULL DEFAULT NULL,
  `aName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aTribe` int(11) NULL DEFAULT NULL,
  `aPreviousTribe` int(11) NULL DEFAULT NULL,
  `aLevel` int(11) NULL DEFAULT NULL,
  `aGeneralExperience` int(11) NULL DEFAULT NULL,
  `aGuildName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aKillOtherTribe` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of RankInfo
-- ----------------------------

-- ----------------------------
-- Table structure for RecommandInfo
-- ----------------------------
DROP TABLE IF EXISTS `RecommandInfo`;
CREATE TABLE `RecommandInfo`  (
  `Island` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `Partition` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `World` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of RecommandInfo
-- ----------------------------
INSERT INTO `RecommandInfo` VALUES ('0', '0', '0');
INSERT INTO `RecommandInfo` VALUES ('0', '0', '1');
INSERT INTO `RecommandInfo` VALUES ('0', '0', '2');
INSERT INTO `RecommandInfo` VALUES ('0', '0', '3');

-- ----------------------------
-- Table structure for Replace_itemList
-- ----------------------------
DROP TABLE IF EXISTS `Replace_itemList`;
CREATE TABLE `Replace_itemList`  (
  `zhengfai` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `xiefai` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mofai` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Replace_itemList
-- ----------------------------

-- ----------------------------
-- Table structure for Replace_SkillCode
-- ----------------------------
DROP TABLE IF EXISTS `Replace_SkillCode`;
CREATE TABLE `Replace_SkillCode`  (
  `zhengfai` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `xiefai` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mofai` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Replace_SkillCode
-- ----------------------------

-- ----------------------------
-- Table structure for Replace_User
-- ----------------------------
DROP TABLE IF EXISTS `Replace_User`;
CREATE TABLE `Replace_User`  (
  `uID` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aTribe` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `reaTribe` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Replace_User
-- ----------------------------

-- ----------------------------
-- Table structure for SecretCardInfo
-- ----------------------------
DROP TABLE IF EXISTS `SecretCardInfo`;
CREATE TABLE `SecretCardInfo`  (
  `cNumber01` int(11) NULL DEFAULT NULL,
  `cNumber02` int(11) NULL DEFAULT NULL,
  `cNumber03` int(11) NULL DEFAULT NULL,
  `cNumber04` int(11) NULL DEFAULT NULL,
  `cNumber05` int(11) NULL DEFAULT NULL,
  `cNumber06` varchar(11) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `cNumber07` int(11) NULL DEFAULT NULL,
  `cNumber08` int(11) NULL DEFAULT NULL,
  `cNumber09` int(11) NULL DEFAULT NULL,
  `cNumber10` int(11) NULL DEFAULT NULL,
  `cNumber11` int(11) NULL DEFAULT NULL,
  `cNumber12` int(11) NULL DEFAULT NULL,
  `cNumber13` int(11) NULL DEFAULT NULL,
  `cNumber14` int(11) NULL DEFAULT NULL,
  `cNumber15` int(11) NULL DEFAULT NULL,
  `cNumber16` int(11) NULL DEFAULT NULL,
  `cNumber17` int(11) NULL DEFAULT NULL,
  `cNumber18` int(11) NULL DEFAULT NULL,
  `cNumber19` int(11) NULL DEFAULT NULL,
  `cNumber20` varchar(11) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `cNumber21` int(11) NULL DEFAULT NULL,
  `cNumber22` int(11) NULL DEFAULT NULL,
  `cNumber23` int(11) NULL DEFAULT NULL,
  `cNumber24` int(11) NULL DEFAULT NULL,
  `cNumber25` int(11) NULL DEFAULT NULL,
  `cNumber26` int(11) NULL DEFAULT NULL,
  `cNumber27` int(11) NULL DEFAULT NULL,
  `cNumber28` int(11) NULL DEFAULT NULL,
  `cNumber29` int(11) NULL DEFAULT NULL,
  `cNumber30` int(11) NULL DEFAULT NULL,
  `cNumber31` int(11) NULL DEFAULT NULL,
  `cNumber32` int(11) NULL DEFAULT NULL,
  `cNumber33` int(11) NULL DEFAULT NULL,
  `cNumber34` int(11) NULL DEFAULT NULL,
  `cNumber35` int(11) NULL DEFAULT NULL,
  `cNumber36` int(11) NULL DEFAULT NULL,
  `cNumber37` int(11) NULL DEFAULT NULL,
  `cNumber38` int(11) NULL DEFAULT NULL,
  `cNumber39` int(11) NULL DEFAULT NULL,
  `cNumber40` int(11) NULL DEFAULT NULL,
  `cNumber41` int(11) NULL DEFAULT NULL,
  `cNumber42` int(11) NULL DEFAULT NULL,
  `cNumber43` int(11) NULL DEFAULT NULL,
  `cNumber44` int(11) NULL DEFAULT NULL,
  `cNumber45` int(11) NULL DEFAULT NULL,
  `cNumber46` int(11) NULL DEFAULT NULL,
  `cNumber47` int(11) NULL DEFAULT NULL,
  `cNumber48` int(11) NULL DEFAULT NULL,
  `cNumber49` int(11) NULL DEFAULT NULL,
  `cNumber50` int(11) NULL DEFAULT NULL,
  `cID` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of SecretCardInfo
-- ----------------------------

-- ----------------------------
-- Table structure for Send_ErrorLog
-- ----------------------------
DROP TABLE IF EXISTS `Send_ErrorLog`;
CREATE TABLE `Send_ErrorLog`  (
  `InDate` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uID` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `Msg` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Send_ErrorLog
-- ----------------------------

-- ----------------------------
-- Table structure for Send_HisTory
-- ----------------------------
DROP TABLE IF EXISTS `Send_HisTory`;
CREATE TABLE `Send_HisTory`  (
  `InDate` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uID` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `beforeCode` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `beforeValue` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `inCode` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `inValue` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `inPosition` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Send_HisTory
-- ----------------------------

-- ----------------------------
-- Table structure for Send_Item
-- ----------------------------
DROP TABLE IF EXISTS `Send_Item`;
CREATE TABLE `Send_Item`  (
  `uID` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `iCode` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `iValue` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Send_Item
-- ----------------------------

-- ----------------------------
-- Table structure for Send_ItemBackUp
-- ----------------------------
DROP TABLE IF EXISTS `Send_ItemBackUp`;
CREATE TABLE `Send_ItemBackUp`  (
  `InDate` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uID` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `MasterInfo_uSaveItem` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `MasterInfo_uSaveItemValue` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `MasterInfo_uSaveItem2` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `MasterInfo_uSaveItem2Value` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of Send_ItemBackUp
-- ----------------------------

-- ----------------------------
-- Table structure for TempGuild
-- ----------------------------
DROP TABLE IF EXISTS `TempGuild`;
CREATE TABLE `TempGuild`  (
  `uid` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aname` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of TempGuild
-- ----------------------------

-- ----------------------------
-- Table structure for TempTbl
-- ----------------------------
DROP TABLE IF EXISTS `TempTbl`;
CREATE TABLE `TempTbl`  (
  `Col002` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `Col003` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of TempTbl
-- ----------------------------

-- ----------------------------
-- Table structure for TribeControl
-- ----------------------------
DROP TABLE IF EXISTS `TribeControl`;
CREATE TABLE `TribeControl`  (
  `aTribeType0Count` int(11) NULL DEFAULT 0,
  `aCreateFlag0` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aTribeType1Count` int(11) NULL DEFAULT 0,
  `aCreateFlag1` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aTribeType2Count` int(11) NULL DEFAULT 0,
  `aCreateFlag2` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `aWorldNumber` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of TribeControl
-- ----------------------------
INSERT INTO `TribeControl` VALUES (1013, '1', 1057, '1', 1116, '1', 1);

-- ----------------------------
-- Table structure for TribeInfo
-- ----------------------------
DROP TABLE IF EXISTS `TribeInfo`;
CREATE TABLE `TribeInfo`  (
  `mTribeVoteInfo` text CHARACTER SET gb2312 COLLATE gb2312_bin NULL,
  `mTribe01Master` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `mTribe01SubMasterInfo` text CHARACTER SET gb2312 COLLATE gb2312_bin NULL,
  `mTribe02Master` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `mTribe02SubMasterInfo` text CHARACTER SET gb2312 COLLATE gb2312_bin NULL,
  `mTribe03Master` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `mTribe03SubMasterInfo` text CHARACTER SET gb2312 COLLATE gb2312_bin NULL,
  `mTribe04Master` varchar(255) CHARACTER SET gb2312 COLLATE gb2312_bin NULL DEFAULT NULL,
  `mTribe04SubMasterInfo` text CHARACTER SET gb2312 COLLATE gb2312_bin NULL,
  `aWorldNumber` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = gb2312 COLLATE = gb2312_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of TribeInfo
-- ----------------------------
INSERT INTO `TribeInfo` VALUES ('00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '', '000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '', '000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '', '000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', '', '000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000', 1);

-- ----------------------------
-- Table structure for uCount
-- ----------------------------
DROP TABLE IF EXISTS `uCount`;
CREATE TABLE `uCount`  (
  `num` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `uCount` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `wDate` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `wTime` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of uCount
-- ----------------------------

-- ----------------------------
-- Table structure for WebMall
-- ----------------------------
DROP TABLE IF EXISTS `WebMall`;
CREATE TABLE `WebMall`  (
  `gGiftItem01` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem01Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem02` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem02Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem03` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem03Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem04` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem04Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem05` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem05Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem06` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem06Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem07` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem07Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem08` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem08Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem09` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem09Cnt` int(11) NULL DEFAULT 0,
  `gGiftItem10` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gGiftItem10Cnt` int(11) NULL DEFAULT 0,
  `gGiftItemSetNumber` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of WebMall
-- ----------------------------

-- ----------------------------
-- Table structure for WebMallItem
-- ----------------------------
DROP TABLE IF EXISTS `WebMallItem`;
CREATE TABLE `WebMallItem`  (
  `gGiftItemSetNumber` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gPurchaseCnt` int(11) NULL DEFAULT 0,
  `gUsedCnt` int(11) NULL DEFAULT 0,
  `gUseID` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NULL DEFAULT NULL,
  `gWebMallSeq` int(11) NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of WebMallItem
-- ----------------------------

-- ----------------------------
-- Table structure for WorldInfo
-- ----------------------------
DROP TABLE IF EXISTS `WorldInfo`;
CREATE TABLE `WorldInfo`  (
  `mZone38WinTribeDate` datetime(0) NULL DEFAULT NULL,
  `mZone38WinTribe` tinyint(4) NULL DEFAULT NULL,
  `mTribe1SymbolDate` datetime(0) NULL DEFAULT NULL,
  `mTribe1Symbol` tinyint(4) NULL DEFAULT NULL,
  `mTribe2SymbolDate` datetime(0) NULL DEFAULT NULL,
  `mTribe2Symbol` tinyint(4) NULL DEFAULT NULL,
  `mTribe3SymbolDate` datetime(0) NULL DEFAULT NULL,
  `mTribe3Symbol` tinyint(4) NULL DEFAULT NULL,
  `mMonsterSymbolDate` datetime(0) NULL DEFAULT NULL,
  `mMonsterSymbol` tinyint(4) NULL DEFAULT NULL,
  `mTribe1Point` int(11) NULL DEFAULT NULL,
  `mTribe2Point` int(11) NULL DEFAULT NULL,
  `mTribe3Point` int(11) NULL DEFAULT NULL,
  `mCloseTime` tinyint(4) NULL DEFAULT NULL,
  `mCloseTribe` tinyint(4) NULL DEFAULT NULL,
  `mFormationCount` varchar(96) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mGetStoneCount` bigint(20) NULL DEFAULT NULL,
  `mLostStoneCount` bigint(20) NULL DEFAULT NULL,
  `mYongpokCount` bigint(20) NULL DEFAULT NULL,
  `mTribe01Master` varchar(3) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe01SubMaster` varchar(93) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe02Master` varchar(8) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe02SubMaster` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe03Master` varchar(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe03SubMaster` varchar(98) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `tNotice01` tinyint(4) NULL DEFAULT NULL,
  `tNotice02` tinyint(4) NULL DEFAULT NULL,
  `tNotice03` tinyint(4) NULL DEFAULT NULL,
  `mTribe4SymbolDate` datetime(0) NULL DEFAULT NULL,
  `mTribe4Symbol` tinyint(4) NULL DEFAULT NULL,
  `mTribe4Point` int(11) NULL DEFAULT NULL,
  `mPossibleAlliance01Date` varchar(19) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance01Info` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance02Date` varchar(19) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance02Info` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance03Date` varchar(19) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance03Info` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance04Date` varchar(19) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mPossibleAlliance04Info` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mAlliance0101` int(11) NULL DEFAULT NULL,
  `mAlliance0102` int(11) NULL DEFAULT NULL,
  `mAlliance0201` int(11) NULL DEFAULT NULL,
  `mAlliance0202` int(11) NULL DEFAULT NULL,
  `mTribe1VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe2VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe3VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe4VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mClose1VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mClose2VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mClose3VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mClose4VoteState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe4QuestDate` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe4QuestState` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe4QuestName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `aWorldNumber` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mAlliance01Date` datetime(0) NULL DEFAULT NULL,
  `mAlliance02Date` datetime(0) NULL DEFAULT NULL,
  `mTribe1VoteDate` datetime(0) NULL DEFAULT NULL,
  `mTribe2VoteDate` datetime(0) NULL DEFAULT NULL,
  `mTribe3VoteDate` datetime(0) NULL DEFAULT NULL,
  `mTribe4VoteDate` datetime(0) NULL DEFAULT NULL,
  `mClose1VoteDate` datetime(0) NULL DEFAULT NULL,
  `mClose2VoteDate` datetime(0) NULL DEFAULT NULL,
  `mClose3VoteDate` datetime(0) NULL DEFAULT NULL,
  `mClose4VoteDate` datetime(0) NULL DEFAULT NULL,
  `mUpdateTime` datetime(0) NULL DEFAULT NULL,
  `mTribeVoteInfo` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe01SubMasterInfo` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe02SubMasterInfo` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe03SubMasterInfo` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `mTribe04SubMasterInfo` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of WorldInfo
-- ----------------------------
INSERT INTO `WorldInfo` VALUES ('2020-02-19 16:56:20', 1, '2020-02-18 18:26:09', 1, '2020-02-18 18:11:46', 1, '2020-02-18 18:49:05', 1, '2020-02-18 17:07:41', 1, 19688, 19688, 19688, 0, -1, '029403410478011001350340007301190260023101751211001500310136021001741158031501650974023302811766', 8902339999, 166402394881, 33901871266, '东1家', '醉酒的午后  绿野魔踪    绿野神宗    xiao潴      Oo敢爱敢恨oO醉卧笑红尘  乁真武之光乁灬天丶後丨  江湖老道    老同学      惩罚丶者    玄武丶惩罚者', 'X丨MAN灬情殇', '剀          花葬Vs先哲  夜色乄孤狼  飘渺雪儿    kIng灬晴朗  苏州知俯    如幻似梦    愤怒的菠萝  浅唱试K歪歪 戰魂乄天使  风神A       Tony丶贾    ', '苏嗏哈尔灿', '茎长大巨含  海皇V波塞顿 Magic梦汐   黑山老妖婆  魔狱乄咆哮爆凤君        力拔山兮盖世阿龙        小鈡乄先生  Mine丶潮汐  原哥        专业杀B     ', 0, 0, 0, '2020-02-18 18:11:46', 3, 35685, '20200225', '1', '0', '0', '20200225', '1', '0', '0', -1, -1, -1, -1, '0', '0', '0', '0', '0', '0', '0', '0', '2013-07-23 19:57:02', '0', 'fast', '1', '2020-02-11 21:57:31', '2020-01-12 17:05:04', '2020-02-11 22:00:00', '2020-02-11 22:00:00', '2020-02-11 22:00:00', '2020-02-11 22:00:00', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

SET FOREIGN_KEY_CHECKS = 1;
