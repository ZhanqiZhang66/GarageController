#Garage Controller with Remote Control
##Genaral:
###Door buttons
-Should start the door’s motion if it’s halted
-Should stop the door’s motion if it’s moving
###Light buttons
-Should turn the light on if it’s off
-Should turn the light off if it’s on
###All UIs (OLED and webpage) should provide a clear indication of state:
-Door opening
-Door closing
-Door stopped / fault (this can be shown in detail or all types of errors and faults can be depicted with a single message)
-Light Status (on/off)

##JavaScript/UI Requirements (Variation on Assignment 4 Requirements):
###Data Communications Design:
-The data communication features supported by Particle (e.g., variables, function calls, and event streams), should be used in appropriate ways.
-Unnecessary polling of information from a remote system could use excessive data when this work is converted to a mobile app, so it is not allowed. (I.e., don’t use a JavaScript timer to repeatedly call functions on the Photon). This can usually be avoided by using a publish-subscribe model (Particle’s Event Streams)
##Light Control:
-Users should be able to turn the lights on/off from the UI
-Users should be able to set the light’s maximum brightness from the UI
-When the light is “on” it will be on at this maximum brightness. For example, if the “maximum” is set to 50% the light will never be on at full power. WHen the light comes on (either due to door motion or a light button) it will be on at this partial brightness. When it fades to off, it will start from this value.
-The “minimum” value of maximum brightness should still be reasonably visible. That is, the users will only be allowed to pick from a range of values that are clearly “on”, but that have varying brightness.
-Users should be able to configure the light auto-off time from the UI
-Use 1s to 60s to simplify testing and demos

##Controller Hardware Requirements (Additions to Assignment 4 Requirements):
###Light Behavior:
-The light should be at the configured “maximum” brightness whenever the door is moving. As soon as the door stops the light’s auto-off timer should start.
-When the light is turned on, it should start at the configured “maximum” brightness.
-The light will fade off over 5s after the auto-off time has elapsed.
-The light should automatically fade off after the “auto-off” time regardless of how it was turned on. (There is no “enable” for the light’s auto-off)
-If the light is on when a light button is hit, the light should turn off immediately (even if the door is in motion or if the light is fading off).
-If the light is off when a light button is hit, the light should turn on and the auto-off timer should start. Example: If the “auto-off” time is 4 seconds and light is turned on (no door motion), the light should stay on at the configured “maximum” brightness for 4 seconds and then fade off over the remaining 5 seconds.
-Fades always start at the configured “maximum” brightness and proceed to a complete off.
-Toggling it while fading will make it turn off immediately.
-Toggling it while it is off will make it turn on immediately and reset/restart the auto-off timer.
-Changing the maximum brightness while the light is on should immediately change the brightness.
-Changing the maximum brightness while the light is fading can be done at your discretion (the maximum setting should be changed, but it doesn’t have to impact the values used for the current fading)

##Remote Hardware & Implementation Requirements :
-Uses a Photon as the primary processor (this assignment uses both Photons)
-Buttons should not appear to bounce. The approach you use for debouncing is at your discretion.
-Includes an OLED screen that shows the garage state. It should clearly show:
  -Door state
  -Light state
-Includes two buttons (or wires to simulate buttons):
 -One to toggle the door state
 -One to toggle the light state
-The Remote should not interact with UIs in any way. It should be able to interact with the controller controller even if the UI isn’t opened/running.
-When the remote starts it should somehow request state information (much like the webpage UI should).
-Like the webpage UI, it should not allow the buttons to impact state until it receives valid state information.
