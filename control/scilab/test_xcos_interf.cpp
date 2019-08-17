#include "scilab/scicos_block4.h"
//#include "scilab/scicos_block.h"  // fixme: why not it?
#include "scilab/scicos.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Goal: connect real code to XCos. Looks hard. Too deep.
//   Stop. Use scilab inc function. Остается вопрос с состояниями
//
//
// http://help.scilab.org/docs/5.5.2/en_US/C_struct.html
// http://www.scicos.org/Newblock.pdf !!!
//
// need define modes and surf - разрыва в производной
//
// fixme: как добавить цифровые
// fixme: как добавить свои непрерывные, если нужны производные?

#define r_IN(n, i) ((GetRealInPortPtrs(blk, n+1))[(i)])
#define r_OUT(n, i) ((GetRealOutPortPtrs(blk, n+1))[(i)])

// parameters
#define Lhi (GetRparPtrs(blk)[0]) // integrator high limit
#define Llo (GetRparPtrs(blk)[1]) // integrator low limit

// inputs
//#define in (r_IN(0,0)) // integrator input
#define gainp (r_IN(1,0)) // integrator gain when X > 0
#define gainn (r_IN(2,0)) // integrator gain when X <= 0

// outputs
#define out (r_OUT(0, 0)) // integrator output
#define Igain (r_OUT(1, 0)) // integrator gain

// states
// fixme: can I have own states?
#define X (GetState(blk)[0]) // integrator state
#define Xdot (GetDerState(blk)[0]) // derivative of the integrator output

//
//
// Spec
// other constants
#define __surf0 (GetGPtrs(blk)[0])
#define __surf1 (GetGPtrs(blk)[1])
#define __surf2 (GetGPtrs(blk)[2])
#define __mode0 (GetModePtrs(blk)[0])

// if X is greater than Lhi, then mode is 1
// if X is between Lhi and zero, then mode is 2
// if X is between zero and Llo, then mode is 3
// if X is less than Llo, then mode is 4

#define __mode_xhzl 1
#define __mode_hxzl 2
#define __mode_hzxl 3
#define __mode_hzlx 4
//
//
//

class FunnyIntegrator
{
public:
	// кажется можно использовать не все
	enum {
		CALC_DERIVATIVE=0,
		CALC_OUT=1,
		CALC_SURF_AND_MODES=9
	};

	FunnyIntegrator( scicos_block *blk )
	{
		this->blk = blk;
	}

	// integrator input
	int getIn() const {
		return r_IN(0,0);
	}
private:
	scicos_block *blk;
};

FunnyIntegrator* g_instance = NULL;

FunnyIntegrator* getInstance(scicos_block *blk)
{
	if ( !g_instance )
		g_instance = new FunnyIntegrator( blk );
	return g_instance;
}

void lim_int(scicos_block *blk, int flag)
{

	switch (flag)
	{
	case FunnyIntegrator::CALC_DERIVATIVE: {
		// compute the derivative of the continuous time state
		double gain = 0;
		double in = getInstance( blk )->getIn();

		if ((__mode0 == __mode_xhzl && in < 0) ||
				__mode0 == __mode_hxzl)
			gain = gainp;
		else if ((__mode0 == __mode_hzlx && in > 0) ||
				__mode0 == __mode_hzxl)
			gain = gainn;
		Xdot = gain * in;
		break;
	}
	case FunnyIntegrator::CALC_OUT:
		// compute the outputs of the block
		if (X >= Lhi || X <= Llo)
			Igain = 0;
		else if (X > 0)
			Igain = gainp;
		else
			Igain = gainn;
			out = X;
		break;

	case FunnyIntegrator::CALC_SURF_AND_MODES:
		// compute zero crossing surfaces and set modes
		__surf0 = X - Lhi;
		__surf1 = X;
		__surf2 = X - Llo;

		if (get_phase_simulation() == 1)
		{
			if (__surf0 >= 0)
				__mode0 = __mode_xhzl;
			else if (__surf2 <= 0)
				__mode0 = __mode_hzlx;
			else if (__surf1 > 0)
				__mode0 = __mode_hxzl;
			else
				__mode0 = __mode_hzxl;
		}
		break;
}
}
