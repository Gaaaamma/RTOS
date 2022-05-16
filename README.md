# RTOS
1102_RTOS Class Labs
## Dependencies
Need to install DOSBOX<br>
<img width="640" alt="截圖 2022-05-16 下午11 40 21" src="https://user-images.githubusercontent.com/84212529/168631331-718e153c-62f8-44bb-aa29-047a3a08a90b.png">

## LAB1
**Implement Rate Monotonic Scheduling.<br>**

One set of periodic tasks<br>
Task(computation_Time,Period)<br>
{ t1(1,3), t2(3,6) }<br>
– Show context switch behaviors <br>
– Show deadline violations if there is any <br>

Sample output be like:<br>
<img width="449" alt="截圖 2022-05-16 下午11 53 56" src="https://user-images.githubusercontent.com/84212529/168633982-03d9e448-7822-49db-a0c8-604dd4f2d76d.png">

### Execution
Note: Since different LAB will modify OS_CORE.c, you should use git reset to reset to the version of LAB just finished. 
```shell=
# Use git reset to change the version to LAB1 DONE
$ git reset b35beda
```
```shell=
Under DOSBOX
$ mount C /path/to/ucOS2
$ C:
$ MOVE.BAT # move to working directory
$ MAKETEST.BAT
$ TEST.EXE > output.txt # Redirect output to .txt file to observe the result 
Press <ESC> until program stopped.
```
```shell=
Under terminal
$ cd /path/to/ucOS2/SOFTWARE/UCOS-II/LAB1/BC45/TEST
$ cat output.txt   # Check Result
```
### Result Overview
Rate Monotonic Scheduling with Task1(1,3) Task2(3,6)<br>
<img width="637" alt="截圖 2022-05-16 下午11 45 06" src="https://user-images.githubusercontent.com/84212529/168632216-4470604b-5967-4ac0-b090-db519c902375.png">


## LAB3
**Implement Ceiling Priority Protocol.<br>**

Simulate the scenario:<br>
<img width="683" alt="截圖 2022-05-17 上午12 01 27" src="https://user-images.githubusercontent.com/84212529/168635714-42060a88-da48-418b-a390-62d93d3b9786.png">

Each task will allocate shared resource at particular time in the above graph.<br>
The scheduling way is based on CPP(Ceiling Priority Protocol).<br>
Try to implement Ceiling Priority Protocol in uCOS-II.<br>


### Execution
Note: Since different LAB will modify OS_CORE.c, you should use git reset to reset to the version of LAB just finished. 
```shell=
# Use git reset to change the version to LAB3 scenario 1 DONE -> remove unnecessary logs
$ git reset 544e889
```
```shell=
Under DOSBOX
$ mount C /path/to/ucOS2
$ C:
$ MOVE.BAT # move to working directory
$ MAKETEST.BAT
$ TEST.EXE > output.txt # Redirect output to .txt file to observe the result 
Press <ESC> until program stopped.
```
```shell=
Under terminal
$ cd /path/to/ucOS2/SOFTWARE/UCOS-II/LAB3/BC45/TEST
$ cat output.txt   # Check Result
```
### Result Overview
Ceiling Priority Protocol<br>
Since we need to simulate the task awaking at the appoointed time, we make OSTime 100 as the start time.<br>
We just want to check if CPP is implemented correctly, so the result will start at OSTime 100 and will only trigger one time.<br>
<img width="571" alt="截圖 2022-05-17 上午12 14 19" src="https://user-images.githubusercontent.com/84212529/168637538-ecf43656-04d9-45b3-ab48-b303dd286956.png">
