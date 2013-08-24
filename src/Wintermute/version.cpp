#include "version.hpp"

using Wintermute::Version;

Version Version::Any = Version();

QDataStream& operator<< ( QDataStream& out, const Wintermute::Version& version )
{
	out << version.toString();
	return out;
}

QDataStream& operator>> ( QDataStream& in, Wintermute::Version& version )
{
	QString versionStr;
	in >> versionStr;
	version = Version::fromString ( versionStr );
	return in;
}
