#!/bin/sh
cd /user/service/bin
sleep 1
./ts2_centerd
sleep 1
./ts2_gamelogd
sleep 1
./ts2_player_userd
sleep 1
./ts2_logind
sleep 1
./ts2_relayd
sleep 1
./ts2_extrad
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_1.conf -l /user/service/logs/ts2_zoned/ts2_zoned_1_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_2.conf -l /user/service/logs/ts2_zoned/ts2_zoned_2_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_3.conf -l /user/service/logs/ts2_zoned/ts2_zoned_3_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_4.conf -l /user/service/logs/ts2_zoned/ts2_zoned_4_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_5.conf -l /user/service/logs/ts2_zoned/ts2_zoned_5_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_6.conf -l /user/service/logs/ts2_zoned/ts2_zoned_6_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_7.conf -l /user/service/logs/ts2_zoned/ts2_zoned_7_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_8.conf -l /user/service/logs/ts2_zoned/ts2_zoned_8_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_9.conf -l /user/service/logs/ts2_zoned/ts2_zoned_9_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_10.conf -l /user/service/logs/ts2_zoned/ts2_zoned_10_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_11.conf -l /user/service/logs/ts2_zoned/ts2_zoned_11_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_12.conf -l /user/service/logs/ts2_zoned/ts2_zoned_12_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_13.conf -l /user/service/logs/ts2_zoned/ts2_zoned_13_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_14.conf -l /user/service/logs/ts2_zoned/ts2_zoned_14_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_15.conf -l /user/service/logs/ts2_zoned/ts2_zoned_15_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_16.conf -l /user/service/logs/ts2_zoned/ts2_zoned_16_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_17.conf -l /user/service/logs/ts2_zoned/ts2_zoned_17_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_18.conf -l /user/service/logs/ts2_zoned/ts2_zoned_18_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_19.conf -l /user/service/logs/ts2_zoned/ts2_zoned_19_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_20.conf -l /user/service/logs/ts2_zoned/ts2_zoned_20_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_21.conf -l /user/service/logs/ts2_zoned/ts2_zoned_21_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_22.conf -l /user/service/logs/ts2_zoned/ts2_zoned_22_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_23.conf -l /user/service/logs/ts2_zoned/ts2_zoned_23_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_24.conf -l /user/service/logs/ts2_zoned/ts2_zoned_24_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_25.conf -l /user/service/logs/ts2_zoned/ts2_zoned_25_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_26.conf -l /user/service/logs/ts2_zoned/ts2_zoned_26_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_27.conf -l /user/service/logs/ts2_zoned/ts2_zoned_27_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_28.conf -l /user/service/logs/ts2_zoned/ts2_zoned_28_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_29.conf -l /user/service/logs/ts2_zoned/ts2_zoned_29_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_30.conf -l /user/service/logs/ts2_zoned/ts2_zoned_30_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_31.conf -l /user/service/logs/ts2_zoned/ts2_zoned_31_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_32.conf -l /user/service/logs/ts2_zoned/ts2_zoned_32_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_33.conf -l /user/service/logs/ts2_zoned/ts2_zoned_33_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_34.conf -l /user/service/logs/ts2_zoned/ts2_zoned_34_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_35.conf -l /user/service/logs/ts2_zoned/ts2_zoned_35_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_36.conf -l /user/service/logs/ts2_zoned/ts2_zoned_36_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_37.conf -l /user/service/logs/ts2_zoned/ts2_zoned_37_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_38.conf -l /user/service/logs/ts2_zoned/ts2_zoned_38_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_39.conf -l /user/service/logs/ts2_zoned/ts2_zoned_39_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_40.conf -l /user/service/logs/ts2_zoned/ts2_zoned_40_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_41.conf -l /user/service/logs/ts2_zoned/ts2_zoned_41_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_42.conf -l /user/service/logs/ts2_zoned/ts2_zoned_42_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_43.conf -l /user/service/logs/ts2_zoned/ts2_zoned_43_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_44.conf -l /user/service/logs/ts2_zoned/ts2_zoned_44_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_45.conf -l /user/service/logs/ts2_zoned/ts2_zoned_45_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_46.conf -l /user/service/logs/ts2_zoned/ts2_zoned_46_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_47.conf -l /user/service/logs/ts2_zoned/ts2_zoned_47_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_48.conf -l /user/service/logs/ts2_zoned/ts2_zoned_48_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_49.conf -l /user/service/logs/ts2_zoned/ts2_zoned_49_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_50.conf -l /user/service/logs/ts2_zoned/ts2_zoned_50_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_51.conf -l /user/service/logs/ts2_zoned/ts2_zoned_51_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_52.conf -l /user/service/logs/ts2_zoned/ts2_zoned_52_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_53.conf -l /user/service/logs/ts2_zoned/ts2_zoned_53_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_54.conf -l /user/service/logs/ts2_zoned/ts2_zoned_54_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_55.conf -l /user/service/logs/ts2_zoned/ts2_zoned_55_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_56.conf -l /user/service/logs/ts2_zoned/ts2_zoned_56_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_57.conf -l /user/service/logs/ts2_zoned/ts2_zoned_57_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_58.conf -l /user/service/logs/ts2_zoned/ts2_zoned_58_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_59.conf -l /user/service/logs/ts2_zoned/ts2_zoned_59_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_60.conf -l /user/service/logs/ts2_zoned/ts2_zoned_60_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_61.conf -l /user/service/logs/ts2_zoned/ts2_zoned_61_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_62.conf -l /user/service/logs/ts2_zoned/ts2_zoned_62_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_63.conf -l /user/service/logs/ts2_zoned/ts2_zoned_63_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_64.conf -l /user/service/logs/ts2_zoned/ts2_zoned_64_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_65.conf -l /user/service/logs/ts2_zoned/ts2_zoned_65_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_66.conf -l /user/service/logs/ts2_zoned/ts2_zoned_66_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_67.conf -l /user/service/logs/ts2_zoned/ts2_zoned_67_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_68.conf -l /user/service/logs/ts2_zoned/ts2_zoned_68_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_69.conf -l /user/service/logs/ts2_zoned/ts2_zoned_69_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_70.conf -l /user/service/logs/ts2_zoned/ts2_zoned_70_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_71.conf -l /user/service/logs/ts2_zoned/ts2_zoned_71_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_72.conf -l /user/service/logs/ts2_zoned/ts2_zoned_72_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_73.conf -l /user/service/logs/ts2_zoned/ts2_zoned_73_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_74.conf -l /user/service/logs/ts2_zoned/ts2_zoned_74_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_75.conf -l /user/service/logs/ts2_zoned/ts2_zoned_75_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_76.conf -l /user/service/logs/ts2_zoned/ts2_zoned_76_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_77.conf -l /user/service/logs/ts2_zoned/ts2_zoned_77_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_78.conf -l /user/service/logs/ts2_zoned/ts2_zoned_78_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_79.conf -l /user/service/logs/ts2_zoned/ts2_zoned_79_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_80.conf -l /user/service/logs/ts2_zoned/ts2_zoned_80_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_81.conf -l /user/service/logs/ts2_zoned/ts2_zoned_81_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_82.conf -l /user/service/logs/ts2_zoned/ts2_zoned_82_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_83.conf -l /user/service/logs/ts2_zoned/ts2_zoned_83_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_84.conf -l /user/service/logs/ts2_zoned/ts2_zoned_84_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_85.conf -l /user/service/logs/ts2_zoned/ts2_zoned_85_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_86.conf -l /user/service/logs/ts2_zoned/ts2_zoned_86_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_87.conf -l /user/service/logs/ts2_zoned/ts2_zoned_87_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_88.conf -l /user/service/logs/ts2_zoned/ts2_zoned_88_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_89.conf -l /user/service/logs/ts2_zoned/ts2_zoned_89_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_90.conf -l /user/service/logs/ts2_zoned/ts2_zoned_90_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_91.conf -l /user/service/logs/ts2_zoned/ts2_zoned_91_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_92.conf -l /user/service/logs/ts2_zoned/ts2_zoned_92_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_93.conf -l /user/service/logs/ts2_zoned/ts2_zoned_93_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_94.conf -l /user/service/logs/ts2_zoned/ts2_zoned_94_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_95.conf -l /user/service/logs/ts2_zoned/ts2_zoned_95_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_96.conf -l /user/service/logs/ts2_zoned/ts2_zoned_96_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_97.conf -l /user/service/logs/ts2_zoned/ts2_zoned_97_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_98.conf -l /user/service/logs/ts2_zoned/ts2_zoned_98_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_99.conf -l /user/service/logs/ts2_zoned/ts2_zoned_99_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_100.conf -l /user/service/logs/ts2_zoned/ts2_zoned_100_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_101.conf -l /user/service/logs/ts2_zoned/ts2_zoned_101_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_102.conf -l /user/service/logs/ts2_zoned/ts2_zoned_102_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_103.conf -l /user/service/logs/ts2_zoned/ts2_zoned_103_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_104.conf -l /user/service/logs/ts2_zoned/ts2_zoned_104_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_105.conf -l /user/service/logs/ts2_zoned/ts2_zoned_105_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_106.conf -l /user/service/logs/ts2_zoned/ts2_zoned_106_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_107.conf -l /user/service/logs/ts2_zoned/ts2_zoned_107_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_108.conf -l /user/service/logs/ts2_zoned/ts2_zoned_108_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_109.conf -l /user/service/logs/ts2_zoned/ts2_zoned_109_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_110.conf -l /user/service/logs/ts2_zoned/ts2_zoned_110_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_111.conf -l /user/service/logs/ts2_zoned/ts2_zoned_111_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_112.conf -l /user/service/logs/ts2_zoned/ts2_zoned_112_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_113.conf -l /user/service/logs/ts2_zoned/ts2_zoned_113_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_114.conf -l /user/service/logs/ts2_zoned/ts2_zoned_114_
sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_115.conf -l /user/service/logs/ts2_zoned/ts2_zoned_115_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_116.conf -l /user/service/logs/ts2_zoned/ts2_zoned_116_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_117.conf -l /user/service/logs/ts2_zoned/ts2_zoned_117_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_118.conf -l /user/service/logs/ts2_zoned/ts2_zoned_118_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_119.conf -l /user/service/logs/ts2_zoned/ts2_zoned_119_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_120.conf -l /user/service/logs/ts2_zoned/ts2_zoned_120_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_121.conf -l /user/service/logs/ts2_zoned/ts2_zoned_121_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_122.conf -l /user/service/logs/ts2_zoned/ts2_zoned_122_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_123.conf -l /user/service/logs/ts2_zoned/ts2_zoned_123_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_124.conf -l /user/service/logs/ts2_zoned/ts2_zoned_124_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_125.conf -l /user/service/logs/ts2_zoned/ts2_zoned_125_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_126.conf -l /user/service/logs/ts2_zoned/ts2_zoned_126_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_127.conf -l /user/service/logs/ts2_zoned/ts2_zoned_127_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_128.conf -l /user/service/logs/ts2_zoned/ts2_zoned_128_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_129.conf -l /user/service/logs/ts2_zoned/ts2_zoned_129_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_130.conf -l /user/service/logs/ts2_zoned/ts2_zoned_130_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_131.conf -l /user/service/logs/ts2_zoned/ts2_zoned_131_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_132.conf -l /user/service/logs/ts2_zoned/ts2_zoned_132_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_133.conf -l /user/service/logs/ts2_zoned/ts2_zoned_133_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_134.conf -l /user/service/logs/ts2_zoned/ts2_zoned_134_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_135.conf -l /user/service/logs/ts2_zoned/ts2_zoned_135_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_136.conf -l /user/service/logs/ts2_zoned/ts2_zoned_136_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_137.conf -l /user/service/logs/ts2_zoned/ts2_zoned_137_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_138.conf -l /user/service/logs/ts2_zoned/ts2_zoned_138_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_139.conf -l /user/service/logs/ts2_zoned/ts2_zoned_139_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_140.conf -l /user/service/logs/ts2_zoned/ts2_zoned_140_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_141.conf -l /user/service/logs/ts2_zoned/ts2_zoned_141_
#sleep 1t
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_142.conf -l /user/service/logs/ts2_zoned/ts2_zoned_142_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_143.conf -l /user/service/logs/ts2_zoned/ts2_zoned_143_
#sleep 1
./ts2_zoned -f /user/service/etc/zone/ts2_zoned_145.conf -l /user/service/logs/ts2_zoned/ts2_zoned_145_
sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_168.conf -l /user/service/logs/ts2_zoned/ts2_zoned_168_
#sleep 1
#./ts2_zoned -f /user/service/etc/zone/ts2_zoned_169.conf -l /user/service/logs/ts2_zoned/ts2_zoned_169_
#sleep 1

