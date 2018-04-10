# Newell Day

## Experiments with Axis cameras for Sunrise Kingdom comission

Relevant commands:

Activate and deactivate a virtual port, specify porrt with the `port=#`

`http://camIP/axis-cgi/virtualinput/deactivate.cgi?schemaversion=1&port=1`

`http://camIP/axis-cgi/virtualinput/activate.cgi?schemaversion=2&port=1`

## Setup PC as NTP server

Did this first but it didn't work: [https://www.interfacett.com/blogs/creating-standalone-ntp-server-windows/ ](https://www.interfacett.com/blogs/creating-standalone-ntp-server-windows/)

Then added an action to allow inbound traffic on UDP port 123 in my firewall following instructions [here](https://docs.microsoft.com/en-us/windows/security/identity-protection/windows-firewall/create-an-inbound-port-rule) but still didn't work.

The followed instructions [here](https://www.youtube.com/watch?v=Fq6xDvPJzrg&index=1&list=PLAyn7hdeRiiedUwWXj-mbyGUF7j8HXr0G&t=17s)

And it worked! The key was to enable inbound and outbound access on port 123. We can add security to this by forcing only certain IP addresses to get access so we're not just a puiblicly accessible NTP server.

