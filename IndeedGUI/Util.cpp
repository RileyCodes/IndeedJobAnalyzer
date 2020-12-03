#include "global.h"
#include "Util.h"


QString Util::GetStatusStringByType(Status status)
{
	switch(status)
	{
	case Status::Stopping:
		return "Stopping";
	case Status::Completed:
		return "Completed";
	case Status::Failed:
		return "Failed";
	case Status::Initialized:
		return "Initialized";
	case Status::Running:
		return "Running";
	case Status::Saved:
		return "Saved";
	}
}
