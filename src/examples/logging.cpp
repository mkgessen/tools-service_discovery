#include <service_discovery/service_discovery.h>

namespace dc = servicediscovery;

int main (int argc, char const* argv[])
{
	dc::Client* client = dc::Client::getInstance();
	dc::ServiceBrowser sbrowser(client, "_sd_test._tcp");
	dc::LocalService serv(client, "MyTestService", "_sd_test._tcp", 10000, std::list<std::string>());
	sleep(4);
	return 0;
}