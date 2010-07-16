#include <cmath>

#include <sot/sotStepObserver.h>
#include <sot-core/vector-roll-pitch-yaw.h>
#include <sot-core/matrix-rotation.h>
#include <sot/sotFactory.h>

SOT_FACTORY_ENTITY_PLUGIN(sotStepObserver,"StepObserver");

sotStepObserver::sotStepObserver( const std::string & name )
  :Entity(name)

  ,leftHandPositionSIN( NULL,"sotStepObserver("+name+")::input(vector)::lefthand" )
  ,rightHandPositionSIN( NULL,"sotStepObserver("+name+")::input(vector)::righthand" )

  ,leftFootPositionSIN( NULL,"sotStepObserver("+name+")::input(matrixhomo)::leftfoot" )
  ,rightFootPositionSIN( NULL,"sotStepObserver("+name+")::input(matrixhomo)::rightfoot" )
  ,waistPositionSIN( NULL,"sotStepObserver("+name+")::input(matrixhomo)::waist" )

  ,referencePositionLeftSOUT( boost::bind(&sotStepObserver::computeReferencePositionLeft,this,_1,_2),
			      leftFootPositionSIN<<leftHandPositionSIN<<rightHandPositionSIN,
			      "sotStepObserver("+name+")::output(vector)::position2handLeft" ) 
  ,referencePositionRightSOUT( boost::bind(&sotStepObserver::computeReferencePositionRight,this,_1,_2),
			       rightFootPositionSIN<<rightHandPositionSIN<<leftHandPositionSIN,
			       "sotStepObserver("+name+")::output(vector)::position2handRight" )
  ,referencePositionWaistSOUT( boost::bind(&sotStepObserver::computeReferencePositionWaist,this,_1,_2),
			       waistPositionSIN<<rightHandPositionSIN<<leftHandPositionSIN,
			       "sotStepObserver("+name+")::output(vector)::position2handWaist" )
{
  sotDEBUGIN(25);

  signalRegistration(getSignals());

  sotDEBUGOUT(25);
}


SignalArray<int> sotStepObserver::getSignals( void )
{
  return (leftHandPositionSIN << leftFootPositionSIN << waistPositionSIN
	  << rightHandPositionSIN << rightFootPositionSIN
	  << referencePositionLeftSOUT << referencePositionRightSOUT
	  << referencePositionWaistSOUT );
}


sotStepObserver::operator SignalArray<int> ()
{
  return getSignals();
}


MatrixHomogeneous&
sotStepObserver::computeRefPos( MatrixHomogeneous& res,
				int timeCurr,
				const MatrixHomogeneous& wMref )
{
  sotDEBUGIN(15);

// Set to 0 to compute a reference frame using both hands. Set to non zero
// value to use the right hand frame as a reference frame (for debug).
#define RIGHT_HAND_REFERENCE 0

#if RIGHT_HAND_REFERENCE

  const MatrixHomogeneous& wMrh = rightHandPositionSIN( timeCurr );
  MatrixHomogeneous refMw; wMref.inverse(refMw);
  refMw.multiply(wMrh, res);

#else

  const MatrixHomogeneous& wMlh = leftHandPositionSIN( timeCurr );
  const MatrixHomogeneous& wMrh = rightHandPositionSIN( timeCurr );

  MatrixHomogeneous refMw; wMref.inverse(refMw);
  MatrixHomogeneous sfMlh; refMw.multiply(wMlh, sfMlh);
  MatrixHomogeneous sfMrh; refMw.multiply(wMrh, sfMrh);

  sotMatrixRotation R;
  VectorRollPitchYaw rpy;

  ml::Vector prh(3); sfMrh.extract(prh);
  sfMrh.extract(R);
  VectorRollPitchYaw rpy_rh; rpy_rh.fromMatrix(R);

  ml::Vector plh(3); sfMlh.extract(plh);
  sfMlh.extract(R);
  VectorRollPitchYaw rpy_lh; rpy_lh.fromMatrix(R);

  rpy.fill(0.);
  rpy(2) = std::atan2(prh(0) - plh(0), plh(1) - prh(1));
  ml::Vector p = .5 * (plh + prh);

  rpy.toMatrix(R);
  res.buildFrom(R, p);

#endif

  sotDEBUGOUT(15);
  return res;
}


MatrixHomogeneous&
sotStepObserver::computeReferencePositionLeft( MatrixHomogeneous& res,
					       int timeCurr )
{
  sotDEBUGIN(15);

  const MatrixHomogeneous& wMref = leftFootPositionSIN( timeCurr );

  sotDEBUGOUT(15);
  return computeRefPos( res,timeCurr,wMref );
}


MatrixHomogeneous&
sotStepObserver::computeReferencePositionRight( MatrixHomogeneous& res,
						int timeCurr )
{
  sotDEBUGIN(15);

  const MatrixHomogeneous& wMref = rightFootPositionSIN( timeCurr );

  sotDEBUGOUT(15);
  return computeRefPos( res,timeCurr,wMref );
}


MatrixHomogeneous&
sotStepObserver::computeReferencePositionWaist( MatrixHomogeneous& res,
						int timeCurr )
{
  sotDEBUGIN(15);

  const MatrixHomogeneous& wMref = waistPositionSIN( timeCurr );

  sotDEBUGOUT(15);
  return computeRefPos( res,timeCurr,wMref );
}


void sotStepObserver::display( std::ostream& os ) const
{
  os << "sotStepObserver <" << getName() <<">:" << std::endl;
}


void sotStepObserver::commandLine( const std::string& cmdLine,
				   std::istringstream& cmdArgs,
				   std::ostream& os )
{
  if( cmdLine == "help" )
  {
    os << "StepObserver: " << std::endl
       << std::endl;
  }
  else { Entity::commandLine( cmdLine,cmdArgs,os); }
}

