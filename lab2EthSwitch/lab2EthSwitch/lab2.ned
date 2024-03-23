import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.EthernetSwitch;
import inet.node.inet.StandardHost;

channel GigabitEthernet extends ned.DatarateChannel
{
    datarate = 1Gbps;
    delay = 1us;
    ber = 1e-10;
}

network Network
{
    @display("bgb=422,304");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=223,31");
        }
        client: StandardHost {
            @display("p=93,161");
        }
        server: StandardHost {
            @display("p=307,161");
        }
        ethernetSwitch: EthernetSwitch {
            @display("p=200,104");
        }
        FTP: StandardHost {
            @display("p=149,241");
        }
        HTTP: StandardHost {
            @display("p=262,225");
        }
        Ftpserver: StandardHost {
            @display("p=307,161");
        }
        Httpserver: StandardHost {
            @display("p=361,111");
        }

    connections:
        ethernetSwitch.ethg++ <--> GigabitEthernet <--> server.ethg++;
        ethernetSwitch.ethg++ <--> GigabitEthernet <--> client.ethg++;
        ethernetSwitch.ethg++ <--> GigabitEthernet <--> FTP.ethg++;
        ethernetSwitch.ethg++ <--> GigabitEthernet <--> HTTP.ethg++;
        ethernetSwitch.ethg++ <--> GigabitEthernet <--> Ftpserver.ethg++;
        ethernetSwitch.ethg++ <--> GigabitEthernet <--> Httpserver.ethg++;
}


