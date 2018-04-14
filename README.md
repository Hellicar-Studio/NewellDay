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

### Supress Windows auto-updates and notifications

I've taken these instructions from my CANvas github repo [here](https://github.com/Hellicar-Studio/CANvas/blob/master/README.md).

1. Disable login request by following the instructions [here](https://www.cnet.com/uk/how-to/automatically-log-in-to-your-windows-10-pc/).
	1. Type **WIN+R** and enter netplwiz into the box.
	2. At the top of the window untick the text box that says "Users must enter a username and password to use this computer".
	3. Click Apply and enter the default computer username and password.
2. Set ethernet network to Metered.
	1. open the Run command by pressing **WIN+R**.
	2. Type regedit to get to the registry
	3. Browse to the following path: `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkList\DefaultMediaCost`
	4. Right-click the DefaultMediaCost folder and selecter Permissions.
	5. On the Security tab click the Advanced button.
	6. Click "Change" next to "TrustedInstaller".
	7. Type "Administrators" and click "Check Names".
	8. Click "OK".
	9. Check "Replace owner on subcontainers and objects"
	10. Click Apply.
	11. Click OK.
	12. On Permissions for DefaultMediaCost, select the Administrators group, and then make sure to check the allow Full control box.
	13. Click Apply.
	14. Click OK.
	15. on DefaultMediaCost double click the Ethernet DWORD Value key and change the value from 1 to 2.
	16. Click OK.
	17. Restart your computer.
3. Set App to auto-Launch.
	1. press **WIN+R** and enter shell:startup.
	2. Enter the folder that comes up and right click and select New > Shortcut.
	3. Add a line to the shortcut that looks like this: "C:/Path/To/program.exe" --flagA --flagB ...
