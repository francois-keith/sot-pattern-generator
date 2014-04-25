# ______________________________________________________________________________
# ******************************************************************************
# A simple Herdt walking pattern generator for Romeo.
# ______________________________________________________________________________
# ******************************************************************************

# from dynamic_graph.sot.romeo.sot_romeo_controller import *
# Create the robot.
from dynamic_graph.sot.dynamics import *
from dynamic_graph.sot.core import *
from dynamic_graph.sot.romeo.robot import *
robot = Robot('ROMEO', device=RobotSimu('ROMEO'))
plug(robot.device.state, robot.dynamic.position)

# Create a solver.
from dynamic_graph.sot.application.velocity.precomputed_tasks import initialize
solver = initialize ( robot )

from dynamic_graph.sot.pattern_generator.walking import CreateEverythingForPG , walkFewSteps
CreateEverythingForPG ( robot , solver )
walkFewSteps ( robot )

#-------------------------------------------------------------------------------
#----- MAIN LOOP ---------------------------------------------------------------
#-------------------------------------------------------------------------------

dt=5e-3
def inc():
    robot.device.increment(dt)



def check(robot, verbose =False):
  result = True
  lf_pose = MatrixHomoToPoseRollPitchYaw('lf_pose')
  plug(robot.leftAnkle.position, lf_pose.sin)

  lf_diff = Substract_of_vector('lf_diff')
  plug(lf_pose.sout, lf_diff.sin1)
  lf_diff.sin2.value = (0.69026, 0.096,  0.067, 0, 0, 0)
  lf_diff.sout.recompute (robot.dynamic.position.time)
  for i in range(0,len(lf_diff.sout.value)):
    if(abs(lf_diff.sout.value[i]) > 1e-5):
      if verbose:
        print "robot.leftAnkle.position: ", lf_pose.sout.value 
        print "                expected: ", lf_diff.sin2.value
        print ""
      result = False


  rf_pose = MatrixHomoToPoseRollPitchYaw('rf_pose')
  plug(robot.rightAnkle.position, rf_pose.sin)

  rf_diff = Substract_of_vector('rf_diff')
  plug(rf_pose.sout, rf_diff.sin1)
  rf_diff.sin2.value = (0.69026,-0.109,  0.067, 0, 0, 0)
  rf_diff.sout.recompute (robot.dynamic.position.time)
  for i in range(0,len(rf_diff.sout.value)):
    if(abs(rf_diff.sout.value[i]) > 1e-5):
      if verbose:
        print "robot.rightAnkle.position ", rf_pose.sout.value
        print "                expected: ", rf_diff.sout.value
        print ""
      result = False



  finalPostion = (0.659, -0.006, 0.840, 0, 0, 0)
  for i in (0,5):
    if(abs(robot.dynamic.position.value[i] - finalPostion[i])) > 1e-3:
      if verbose:
        print "error in position "
        print "position: ", robot.dynamic.position.value
        print "expected: ", finalPostion
      result = False


  if result == True:
    print "success"

  return result


for i in range(1,1040):
  inc()

result = check(robot, True)
exit(not result)

