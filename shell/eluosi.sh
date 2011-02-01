#!/bin/sh
aBox0_0=(0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0)
aBox0_1=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0)
aBox0_2=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 -1)
aBox0_3=(0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0)

aBox1_0=(0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0)
aBox1_1=(0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0)
aBox1_2=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 -1)
aBox1_3=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0)

aBox2_0=(0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0)
aBox2_1=(0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0)

aBox3_0=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0)
aBox3_1=(0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0)

aBox4_0=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 0 0 -1)
aBox4_1=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0)

aBox5_0=(0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0)

aBox6_0=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0)
aBox6_1=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0)
aBox6_2=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 -1)
aBox6_3=(0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0)

iSumType=7	#方块类型总数
aStyles=(4 4 2 2 2 1 4)	#各个种类方块样式数
iOrigoX=10	#显示起始位置x
iOrigoY=5	#显示起始位置y
iMapWidth=12	#方块宽10
iMapHeight=15   #方块高15
iBox_X=0	#方块位置x
iBox_Y=0	#方块位置y
iScore=0	#分数
iLevel=0	#级别
iScoreEachLevle=50	#每升一级需积累分数

#初始化所有背景方块
for((i = 0;i<iMapWidth*iMapHeight;i++))
do
  aBoxBg[$i]=0
done

#定义控制信号,挪用信号25~31
iSIG=0           #
iSIG_Rotate=25   #旋转
iSIG_Left=26     #向左移动
iSIG_Right=27    #向右移动
iSIG_Down=28     #向下
iSIG_FallDown=29 #直落到底
iSIG_Exit=30     #退出
iSIG_Pause=31    #暂停

Control()
{
  trap "Game_Exit" 2 3 15 $iSIG_Exit
  echo -ne "\033[?25l"
  SAVEDSTTY=`stty -g`
  iDisplayPID=$1
  while :
  do
  read -s -n 1 LINE
  iSIG=0
  case $LINE in
  "A" )
    iSIG=$iSIG_Rotate
    ;;
  "B" )
    iSIG=$iSIG_Down
    ;;
  "D" )
    iSIG=$iSIG_Left
    ;;
  "C" )
    iSIG=$iSIG_Right
    ;;
  "")
    iSIG=$iSIG_FallDown
    ;;
  p|P)
    iSIG=$iSIG_Pause
    ;;
  q|Q )
    Game_Exit
    ;;
  esac
  [ $iSIG -ne 0 ] && kill -$iSIG $iDisplayPID
  done
}

Display()
{
  trap "iSIG=$iSIG_Rotate"   $iSIG_Rotate
  trap "iSIG=$iSIG_Down"     $iSIG_Down
  trap "iSIG=$iSIG_Left"     $iSIG_Left
  trap "iSIG=$iSIG_Right"    $iSIG_Right
  trap "iSIG=$iSIG_FallDown" $iSIG_FallDown
  trap "iSIG=$iSIG_Pause"    $iSIG_Pause
  local iSIGThis i
  clear
  
  Draw_info
  Create_Box
  while :
  do
  if [ $iSIG -ne $iSIG_Pause ];then
    for((i=0;i<20-iLevel;i++))
    do
      iSIG_Display=$iSIG
      iSIG=0
      case $iSIG_Display in
      $iSIG_Rotate)
        Move_Rotate
        ;;
      $iSIG_Down)
        Move_Down
        ;;
      $iSIG_Left)
        Move_Left
        ;;
      $iSIG_Right)
        Move_Right
       ;;
      $iSIG_FallDown)
        Move_FallDown
        ;;
      $iSIG_Pause)
        iSIG=$iSIG_Pause
        break 
        ;;
      esac
    sleep 0.02
    done
    Move_Down
  fi
  done
}

Game_Exit()
{
  echo -ne "\033[21;1Hgame over\n"
  echo -ne "\033[0m\033[?25h"
  stty $SAVEDSTTY
  kill -9 $iDisplayPID
  exit 0
}
  
Draw_Line()
{
  echo -ne "\033[$1;$((iOrigoX))H--====================--"	#iOrigoY
}
    
Draw_Row()
{
  local i
  for((i=iOrigoY;i<iOrigoY+15;i++))
  do
    echo -ne "\033[$i;$1H||"
  done
}

#显示程序主画面
Draw_info()
{
  echo -ne "\033[45m"
  Draw_Line $((iOrigoY-1))
  Draw_Line $((iOrigoY+iMapHeight))
  Draw_Row $((iOrigoX))	#iOrigoY+(3*2)-2
  Draw_Row $((iOrigoX+2+20))  #iOrigoY+(3*2)+10*2
  echo -ne "\033[0m"
  echo -ne "\033[$((iOrigoY+8));$((iOrigoX+25))H   SCORE"
  echo -ne "\033[$((iOrigoY+10));$((iOrigoX+30))H0"
  echo -ne "\033[$((iOrigoY+13));$((iOrigoX+25))H   LEVEL"
  echo -ne "\033[$((iOrigoY+15));$((iOrigoX+30))H0"
}
  
Draw_Box_Clear()
#把方块隐藏掉
{
  local i
  for((i=0;i<$((${#aBoxMove[*]}-1));i++))
  do
    [ ${aBoxMove[$i]} -ne 0 ] && echo -ne "\033[$(((i/12)+iBox_Y+iOrigoY));$((((i%12)*2)+iOrigoX))H  "
  done
}
  
Draw_Box_New()
#显示方块
{
  local i
  for((i=0;i<$((${#aBoxMove[*]}-1));i++))
  do
    [ ${aBoxMove[$i]} -ne 0 ] && echo -ne "\033[1m\033[${iCurrentColor}m\033[$(((i/12)+iBox_Y+iOrigoY));$((((i%12)*2)+iOrigoX))H[]\033[0m"
  done
}
  
IsBottom()
#检查方块是否下落到底
{
  local i
  if [ $(($iBox_Y+(${#aBoxMove[*]}/12))) -eq 15 ];then
    return 0
  fi
  iBox_Y=$((iBox_Y+1))	#虚拟下移一行
  for((i=0;i<${#aBoxMove[*]};i++))
  do
  [ $((iBox_Y*12+i)) -lt 0 ] && continue	#排除刚开始显示方块时iBox_Y为负的情况
    if [ "${aBoxMove[$i]}" == "1" -a "${aBoxBg[$((iBox_Y*12+i))]}" != "0" ];then	#触底
      iBox_Y=$((iBox_Y-1))	#复原
      return 0
    fi
  done
  iBox_Y=$((iBox_Y-1))
  return 1
}
  
Box_Delete()
#消行.计算分数.
{
  local i j k line
  aLineFull=() 
  aBoxBgNew=()
  line=0
  k=0

  #检查那些行可以消掉
  for((i=0;i<15;i++))	#y
  do
    for((j=0;j<10;j++))
      do
        [ ${aBoxBg[$((i*12+1+j))]} -eq 0 ] && break
        [ $j -eq 9 ] && aLineFull[$line]=$i && (( line++ ))
      done
  done
  [ $line -eq 0 ] && return 0

  #计算分数和级别
  iScore=$((iScore+(line*2)-1))
  echo -ne "\033[15;$((iOrigoX+30))H${iScore}         "
  if [[ $((iScore/iScoreEachLevle)) -gt $iLevel && $iLevel -lt 20 ]]
  then
     (( iLevel++ ))
  fi
  echo -ne "\033[20;$((iOrigoX+30))H${iLevel}        \033[0m"
  
  #重新整理aBoxBg数组
  for((i=0;i<15;i++))
  do
    while [ "$((i+k-line))" == "${aLineFull[$k]}" ] #这里处理连续消除多行
    do
      (( k++ ))
    done
    for((j=0;j<12;j++))
    do
      if [ $i -lt $line ];then
        aBoxBgNew[$((i*12+j))]=0
      else
        aBoxBgNew[$((i*12+j))]=${aBoxBg[$(((i+k-line)*12+j))]}
      fi
    done
  done
  
  #刷新背景
  aBoxBg=(${aBoxBgNew[*]})
  for((i=0;i<15;i++))     #y
  do
    for((j=0;j<10;j++))
      do
        echo -ne "\033[$((i+iOrigoY));$((((j+1)*2)+iOrigoX))H"
        if [ ${aBoxBg[$((i*12+1+j))]} -eq 0 ];then
           echo "  "
        else
           echo -ne "\033[1m\033[${aBoxBg[$((i*12+1+j))]}m[]\033[0m"
        fi
      done
  done
} 
  
Create_Box()
#显示新方块
{
  aBoxMove=()
  if [ -z "$iCurrentType" ];then
    (( iCurrentType = RANDOM % $iSumType ))
    (( iCurrentStyle = RANDOM % ${aStyles[$iCurrentType]} ))
    (( iCurrentColor = 41 + RANDOM % 7 ))
  else
    iCurrentType=$iNewType
    iCurrentStyle=$iNewStyle
    iCurrentColor=$iNewColor
  fi
  #iCurrentType=4
  #iCurrentStyle=1
  aBoxMove=( `eval 'echo ${aBox'$iCurrentType'_'$iCurrentStyle'[*]}'` )
  iBox_Y=${aBoxMove[$((${#aBoxMove[*]}-1))]}
  iBox_X=0
  Draw_Box_New
  Random_Box
  IsBottom && kill -$iSIG_Exit $PPID
}
  
Random_Box()
#随机产生方块并预显示
{
  local i
  (( iNewType = RANDOM % $iSumType ))
  (( iNewStyle = RANDOM % ${aStyles[$iNewType]} ))
  (( iNewColor = 41 + RANDOM % 7 ))

  #抹掉原显示
  for((i=0;i<${#aBoxMove[*]};i++))
  do
    if [ ${aBoxMove[$i]} -eq 1 ];then
    echo -ne "\033[$(($i/12+iOrigoY));$((($i%12)*2+iOrigoX+20))H  "
  fi
  done

  #预显示新方块
  aBoxNEW=( `eval 'echo ${aBox'$iNewType'_'$iNewStyle'[*]}'` )
  for((i=0;i<${#aBoxNEW[*]};i++))
  do
    if [ ${aBoxNEW[$i]} -eq 1 ];then
    echo -ne "\033[1m\033[${iNewColor}m\033[$(($i/12+iOrigoY));$((($i%12)*2+iOrigoX+20))H[]\033[0m"
  fi
  done
}
  
Move_Left()
{
  local i
  #判断是否碰墙
  for((i=0;i<$((${#aBoxMove[*]}/12));i++))
  do
    [ ${aBoxMove[$((i*12+1))]} -eq 1 ] && return 0
  done
  #判断是否碰背景
  for((i=0;i<${#aBoxMove[*]};i++))
  do
    [ $((iBox_Y*12+i)) -le 0 ] && continue   #排除刚开始显示方块时iBox_Y为负的情况
    [ ${aBoxMove[$i]} -eq 1 -a ${aBoxBg[$((iBox_Y*12+i-1))]} -ne 0 ] && return 0 #触底
  done
  #清除原显示
  Draw_Box_Clear
  for((i=0;i<$((${#aBoxMove[*]}-2));i++))
  do
    aBoxMove[$i]=${aBoxMove[$((i+1))]}
  done
  #重新显示方块
  Draw_Box_New
  iBox_X=$((iBox_X-1))
}
  
Move_Right()
#右移
{
  local i
  #判断是否碰墙
  for((i=0;i<$((${#aBoxMove[*]}/12));i++))
  do
  [ ${aBoxMove[$((i*12+10))]} -eq 1 ] && return 0
  done
  #判断是否碰背景
  for((i=0;i<${#aBoxMove[*]};i++))
  do
    [ $((iBox_Y*12+i)) -lt 0 ] && continue   #排除刚开始显示方块时iBox_Y为负的情况
    [ "${aBoxMove[$i]}" == "1" -a "${aBoxBg[$((iBox_Y*12+i+1))]}" != "0" ] && return 0 #触底
  done
  #清除原显示
  Draw_Box_Clear
  for((i=$((${#aBoxMove[*]}-2));i>0;i--))
  do
    aBoxMove[$i]=${aBoxMove[$((i-1))]}
  done
  
  #重新显示方块
  Draw_Box_New
  iBox_X=$((iBox_X+1))
}
 
Move_Down()
#下落一格
{
  local i
  if ! IsBottom	#返回0则表示底部
  then
    Draw_Box_Clear
    iBox_Y=$((iBox_Y+1))
    #画新方块
    Draw_Box_New
  else
  #	把当前方块移到背景 (把方块位置的背景设置为颜色数)(注:0为空)
  for((i=0;i<${#aBoxMove[*]};i++))
  do
    if [ ${aBoxMove[$i]} -eq 1 ];then
      aBoxBg[$((iBox_Y*12+i))]=$iCurrentColor
    fi
  done
    Box_Delete
    Create_Box
  fi
}
  
Move_FallDown()
#直落到底
{
  local i
  Draw_Box_Clear
  for((i=0;i<15;i++))
  do
    if ! IsBottom
    then
      iBox_Y=$((iBox_Y+1))
    else
      break
    fi
  done
  Draw_Box_New
  Move_Down
}

Move_Rotate()
#当前style跳转到下一style ,x位移量根据iBox_X位置决定
{
  local i
  [ ${aStyles[$iCurrentType]} -eq 1 ] && return 0	#只有一种类型的不处理
  [ $iBox_Y -lt 0 ] && return 0 #iBox_Y<0不处理
  sRotateStyle=$(((iCurrentStyle+1)%${aStyles[$iCurrentType]}))
  aBoxRotate=( `eval 'echo ${aBox'$iCurrentType'_'$sRotateStyle'[*]}'` )
  #先根据iBox_X作左右移位处理,把重新赋值aBoxRotate/aBoxMove
  if [ $iBox_X -gt 0 ];then	#右移
    for((i=$((${#aBoxRotate[*]}-2));i>=iBox_X;i--)) #原数组末尾增加了一位数记录起始位置,所以这里要减2
    do
      aBoxRotate[$i]=${aBoxRotate[$((i-iBox_X))]}
    done
    
    for((i=0;i<iBox_X;i++))
    do
      aBoxRotate[$i]=0
    done
  else	#左移
    for((i=0;i<$((${#aBoxRotate[*]}+$iBox_X-2));i++))
    do
      aBoxRotate[$i]=${aBoxRotate[$((i-iBox_X))]}
    done
  
    for((i=$((${#aBoxRotate[*]}+$iBox_X-2));i<$((${#aBoxRotate[*]}-1));i++))
    do
      aBoxRotate[$i]=0
    done
  fi
  
  #判断旋转后是否能放得下
  #1.判断是否碰墙
  for((i=0;i<$((${#aBoxRotate[*]}/12));i++))
  do
    [ ${aBoxRotate[$((i*12))]} -eq 1 ] && return 0
    [ ${aBoxRotate[$((i*12+11))]} -eq 1 ] && return 0
  done
  #2.判断是否触底
  [ $(($iBox_Y+(${#aBoxRotate[*]}/12))) -gt 15 ] && return 0 #旋转后触底
  for((i=0;i<${#aBoxRotate[*]};i++))
  do
  #set -xv
  [ $((iBox_Y*12+i)) -lt 0 ] && continue   #排除刚开始显示方块时iBox_Y为负的情况
    [ "${aBoxRotate[$i]}" == "1" -a "${aBoxBg[$((iBox_Y*12+i))]}" != "0" ] && return 0  #触底
  #set +xv
  done
  
  #可以旋转,消除原方块,显示新方块
  Draw_Box_Clear
  aBoxMove=()
  aBoxMove=(${aBoxRotate[*]})
  iCurrentStyle=$sRotateStyle
  aBoxRotate=()
  Draw_Box_New
}
  
#主程序从这里开始
if [ "$1" != "display" ]
then
  bash $0 display&
  Control $!
  exit
else
  Display
  exit
fi
