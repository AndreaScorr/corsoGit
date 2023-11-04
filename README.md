Research track 1 First assignment
==================
Andrea Scorrano

id: 6463777

Write a python node that controls the robot to put all the golden boxes together.

Installing and running
----------------------

The simulator requires a Python 2.7 installation, the [pygame](http://pygame.org/) library, [PyPyBox2D](https://pypi.python.org/pypi/pypybox2d/2.1-r331), and [PyYAML](https://pypi.python.org/pypi/PyYAML/).

Pygame, unfortunately, can be tricky (though [not impossible](http://askubuntu.com/q/312767)) to install in virtual environments. If you are using `pip`, you might try `pip install hg+https://bitbucket.org/pygame/pygame`, or you could use your operating system's package manager. Windows users could use [Portable Python](http://portablepython.com/). PyPyBox2D and PyYAML are more forgiving, and should install just fine using `pip` or `easy_install`.

## How to Run the code
To run one or more scripts in the simulator, use `run.py`, passing it the file names. 

In this case write this line on the terminal:

```bash
$ python2 run.py assignment.py
```

Robot API
---------

The API for controlling a simulated robot is designed to be as similar as possible to the [SR API][sr-api].

### Motors ###

The simulated robot has two motors configured for skid steering, connected to a two-output [Motor Board](https://studentrobotics.org/docs/kit/motor_board). The left motor is connected to output `0` and the right motor to output `1`.

The Motor Board API is identical to [that of the SR API](https://studentrobotics.org/docs/programming/sr/motors/), except that motor boards cannot be addressed by serial number. So, to turn on the spot at one quarter of full power, one might write the following:

```python
R.motors[0].m0.power = 25
R.motors[0].m1.power = -25
```

### The Grabber ###

The robot is equipped with a grabber, capable of picking up a token which is in front of the robot and within 0.4 metres of the robot's centre. To pick up a token, call the `R.grab` method:

```python
success = R.grab()
```

The `R.grab` function returns `True` if a token was successfully picked up, or `False` otherwise. If the robot is already holding a token, it will throw an `AlreadyHoldingSomethingException`.

To drop the token, call the `R.release` method.

Cable-tie flails are not implemented.

### Vision ###

To help the robot find tokens and navigate, each token has markers stuck to it, as does each wall. The `R.see` method returns a list of all the markers the robot can see, as `Marker` objects. The robot can only see markers which it is facing towards.

Each `Marker` object has the following attributes:

* `info`: a `MarkerInfo` object describing the marker itself. Has the following attributes:
  * `code`: the numeric code of the marker.
  * `marker_type`: the type of object the marker is attached to (either `MARKER_TOKEN_GOLD`, `MARKER_TOKEN_SILVER` or `MARKER_ARENA`).
  * `offset`: offset of the numeric code of the marker from the lowest numbered marker of its type. For example, token number 3 has the code 43, but offset 3.
  * `size`: the size that the marker would be in the real game, for compatibility with the SR API.
* `centre`: the location of the marker in polar coordinates, as a `PolarCoord` object. Has the following attributes:
  * `length`: the distance from the centre of the robot to the object (in metres).
  * `rot_y`: rotation about the Y axis in degrees.
* `dist`: an alias for `centre.length`
* `res`: the value of the `res` parameter of `R.see`, for compatibility with the SR API.
* `rot_y`: an alias for `centre.rot_y`
* `timestamp`: the time at which the marker was seen (when `R.see` was called).




 
Structure of the code 
----------------------
The program is created with 6 functions, so it's possible to have a clear code and use the advantage of modularity:

* ```python
  def drive(speed, seconds):
  ```
  Move the robot with `speed` for `seconds`.
  
* ```python
  def turn(speed, seconds):
  ```
  Turn tje robot with `speed` for `seconds`.
  
* ```python
  def find_golden_token():
  ```
  Use the metod `R.see()`to get a list of all the markers the robot can see, scroll the list and return the distance, the angle and the code of the nearest token if exists, otherwise returns -1.

* ```python
  def init():
  ```
  This function moves the robot in the a specified position (in this case the center), turn around to observe available tokens and put them in a list. This function return two copies of
 the same list that will be  used in the next functions.

* ```python
  def bringBoxNearTheGrayArea(tokenToApproach):
  ```
  This function let the robot to grab the boxes and put them near the gray area.

  
* ```python
  def alignToghetherBoxes(tokenToAlignTogether):
  ```
  Once the `bringBoxNearTheGrayArea(tokenToApproach)` has placed the boxes near the gray area, this function aligns the robot together.
## Pseudocode of functions ##

### drive(speed, seconds): ###
```
  PROCEDURE drive(speed, seconds)
    /*
    Function for setting a linear velocity

    Parameters:
        speed (INTEGER): the speed of the wheels
        seconds (INTEGER): the time interval
    */

    R.motors[0].m0.power = speed
    R.motors[0].m1.power = speed
    Sleep(seconds)
    R.motors[0].m0.power = 0
    R.motors[0].m1.power = 0
 END PROCEDURE
```

### def turn(speed, seconds): ###
```
PROCEDURE turn(speed, seconds)
    /*
    Function for setting an angular velocity

    Parameters:
        speed (INTEGER): the speed of the wheels
        seconds (INTEGER): the time interval
    */

    set motor m0.power = speed
    set motor m1.power = -speed
    Sleep(seconds)
    set motor m0.power = 0
    set motor m1.power = 0
END PROCEDURE
```

### find_golden_token(): ###
```
  PROCEDURE find_golden_token()
    /*
    Function to find the closest golden token

    Returns:
        dist (REAL): distance of the closest golden token (-1 if no golden token is detected)
        rot_y (REAL): angle between the robot and the golden token (-1 if no golden token is detected)
        codeToken (REAL): code of the token
    */

    dist = 100
    PRINT(LENGTH(R.see()))

    FOR EACH token IN R.see() DO
        IF token.dist < dist AND token_marker_is_gold THEN
            dist = token_dist
            rot_y = token_rot_y
            codeToken = token_code
        END IF
    END FOR

    IF dist = 100 THEN
        RETURN -1, -1, -1
    ELSE
        RETURN dist, rot_y, codeToken
    END IF
END PROCEDURE
```

### init(): ###
```
  PROCEDURE InitializeRobot():

    # Drive the robot forward for 5 units at a speed of 60 (set around the center)
    go_straight

    # Initialize an empty list to store tokens
    tokens = []

    # Turn around to observe available tokens
    FOR i in range(0, 12):
        # Print information about tokens in the environment
        Print(R.see(), '\n')
        
        # Turn the robot counterclockwise by 10 Speed for 1 second
        turn_left

        # Iterate through the list of tokens visible to the robot
        FOR each token in R.see():
            # Check if the token's code is not already in the list of tokens
            IF token_code_is_not_already_in_the_list:
                # Add the token's code to the list
                tokens.append(token.info.code)

    # Print the list of collected tokens
    Print(tokens)

    # Create two copies of the list of tokens for different purposes
    tokenToApproach = CopyList(tokens)
    tokenToAlignTogether = CopyList(tokens)

    # Return the lists of tokens for approaching and alignment
    Return tokenToApproach, tokenToAlignTogether
  END PROCEDURE
```

### bringBoxNearTheGrayArea(tokenToApproach): ###
```
  PROCEDURE bringBoxNearTheGrayArea(tokenToApproach)

    # While there are tokens left in the ToDoList
    WHILE not IsListEmpty(tokenToApproach):
        # Find the distance, rotation angle, and code of the nearest golden token
        distance, rotation_angle, codeToken = find_golden_token()
        
        # If no token is detected, turn the robot to the left
        IF no_marker_Detected:
            Print("I don't see any token!!")
            TurnLeftSlightly()
        # IF we are close to the token, attempt to grab it
        ELSEIF distance < d_th:
            Print("Found it!", codeToken)
            
            # If the robot successfully grabs the token, perform the following actions
            IF Robot_has_grabbed:
                Print("Gotcha!")
                MoveRobotBackward()
                ReleaseTheBox()
                TurnLeftSlightly()
                # Remove the token from the ToDoList
                tokenToApproach.remove(codeToken)
                Print('Remaining tokens (ToDoList):', tokenToApproach)
            ELSE:
                Print("Aww, I'm not close enough.")
        # IF the robot is well aligned with the token, move forward
        ELSEIF -a_th <= rotation_angle <= a_th:
            Print("Ah, that'll do.")
            MoveRobotForward()
        # If the robot is not well aligned with the token, adjust its position
        ELSEIF rotation_angle < -a_th:
            Print("Left a bit...")
            TurnLeftSlightly()
        ELSEIF rotation_angle > a_th:
            Print("Right a bit...")
            TurnRightSlightly()
 END PROCEDURE
```
### alignToghetherBoxes(tokenToAlignTogether): ###
```
  PROCEDURE alignToghetherBoxes(tokenToAlignTogether)
   # While there are tokens left in the ToDoList
    WHILE not IsListEmpty(tokenToAlignTogether):
        # Find the distance, rotation angle, and code of the nearest golden token
        distance, rotation_angle, codeToken = find_golden_token()
        
        # If no token is detected, turn the robot to the left
        IF no_marker_detected:
            Print("I don't see any token!!")
            TurnLeftSlightly()
        # if we are close to the token, attempt to grab it
        ELSEIF distance < d_th:
            Print("Found it!")
            
            # If the robot successfully grabs the token, perform the following actions
            IF Robot_has_grabbed:
                Print("Gotcha!")
                TurnLeft(21, 2)  # Turn left with 21 speed for 2 seconds
                DriveForward(21, 3)  # Move forward with 21 speed for 3 seconds
                ReleaseTheBox()
                DriveBackward(21, 3)  # Move backward whit 21 speed for 3 seconds
                TurnRight(21, 2)  # Turn right by with 21 speed for 3 seconds
                # Remove the token from the ToDoList
                tokenToAlignTogether.remove(codeToken)
                Print('Remaining tokens (ToDoList):', tokenToAlignTogether)
            ELSE:
                Print("Aww, I'm not close enough.")
        # IF the robot is well aligned with the token, move forward
        ELSEIF -a_th <= rotation_angle <= a_th:
            Print("Ah, that'll do.")
            DriveForward(20, 0.5)  # Move forward with 20 speed for 0.5 seconds
        # If the robot is not well aligned with the token, adjust its position
        ELSEIF rotation_angle < -a_th:
            Print("Left a bit...")
            TurnLeftSlightly()
        ELSEIF rotation_angle > a_th:
            Print("Right a bit...")
            TurnRightSlightly()
 END PROCEDURE
```

## Results ##

To have everything compact these three functions were invoked in the script:
```python
tokenToApproach, tokenToAlignTogether=init()
bringBoxNearTheGrayArea(tokenToApproach)        
alignToghetherBoxes(tokenToAlignTogether)
print("work done!")
```

### Init phase ###

![alt text](https://github.com/AndreaScorr/corsoGit/blob/main/init.png?raw=true)

The robot is placed in the center

### Approaching phase ###

![alt text](https://github.com/AndreaScorr/corsoGit/blob/main/nearBox.png?raw=true)

The robot has putted the boxes near the gray area.


### Alignment phase ###
![alt text](https://github.com/AndreaScorr/corsoGit/blob/main/alignedBox.png?raw=true)

The robot has aligned the boxes to the center of the area

### TimeLaps  of the simulation ###
The robot is a little bit slow, to speed up the view of the results here it is a video that shows how the robot works.


https://github.com/AndreaScorr/corsoGit/assets/40230364/3711bfbc-62d0-4054-b331-74cef737a7e4




## Possible improvements ##
This robot has the objective to put together the boxes. To reach the goal, the robot put itself in the middle of the gray area, turn around itself to see the boxes that has to put togheter, put them near the gray area, and do a sort of a circle to put the boxes in the middle.
Future developments could be an increase of the execution speed, and implements the software on a real robot. Maybe the method `R.see()` could be replaced by a camera and using computer vision algoritms to detect the gold boxes and the mehod `R.grab()` could be replaced by real end effector, fit for purpose.




  
  

  




