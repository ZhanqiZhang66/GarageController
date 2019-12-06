var onOffSwitch
var onOffSwitchLabel

//Variable showing on the advanced control screen
var autoOffSwitch
var autoOffSwitchLabel
var autoCloseRange
var autoLightRange
var brightnessRange

////Showing Certain Pages
 function signUpButton(){
  document.getElementById("sign-up").hidden = false;
  document.getElementById("login").hidden = true;
  document.getElementById("home").hidden = true;
  document.getElementById("advance-control").hidden = true;
}

function logInButton() {
  document.getElementById("sign-up").hidden = true;
  document.getElementById("login").hidden = false;
  document.getElementById("home").hidden = true;
  document.getElementById("advance-control").hidden = true;
}
function homeButton() {
  document.getElementById("sign-up").hidden = true;
  document.getElementById("login").hidden = true;
  document.getElementById("home").hidden = false;
  document.getElementById("advance-control").hidden = true;
}
function advanceControlButton() {
  document.getElementById("sign-up").hidden = true;
  document.getElementById("login").hidden = true;
  document.getElementById("home").hidden = true;
  document.getElementById("advance-control").hidden = false;
}

/*setInterval(function(){
    document.getElementById("onOffSwitch").innerHTML = garage.printGarageState()
    //document.getElementById("lightOnOffSwitch").innerHTML = garage.lightOn
}, 100)*/

// On startup:  Things to do once the page is fully loaded and the DOM is configured
document.addEventListener("DOMContentLoaded", function(event) {

  onOffSwitch = document.getElementById("onOffSwitch")
  onOffSwitchLabel = document.getElementById("onOffSwitchLabel")

  lightOnOffSwitch = document.getElementById("lightOnOffSwitch")
  lightOnOffSwitchLabel = document.getElementById("lightOnOffSwitchLabel")

  // Variables representing elements on the "Settings" screen
  autoOffSwitch = document.getElementById("autoOffSwitch")
  autoOffSwitchLabel = document.getElementById("autoOffSwitchLabel")
  autoCloseRange = document.getElementById("autoCloseRange")
  autoCloseTimerText = document.getElementById("autoCloseTimerText")

  autoLightRange = document.getElementById("autoLightRange")
  brightnessRange = document.getElementById("brightnessRange")
  lightAutoCloseTimerText = document.getElementById("lightAutoCloseTimerText")
  brightnessSliderText = document.getElementById("brightnessSliderText")
  // Event handlers
  onOffSwitch.addEventListener("click", DoorOnOffButtonClicked)
  lightOnOffSwitch.addEventListener("click", lightButtonClicked)

  autoOffSwitch.addEventListener("checked", DoorAutoCloseButtonClicked)
  autoOffTimeSlider.addEventListener("change", autoOffSliderChange)

  lightautoOffTimeSlider.addEventListener("change", lightAutoOffSliderChange)
  brightness.addEventListener("change", brightnessChange)


  garage.setupChangeListener(stateUpdate)
})

function stateUpdate(newState) {
  console.log("state changed");
  console.log(newState)

  //main control
  onOffSwitch.innerHTML = garageState[parseInt(newState.cur_state)]
  lightOnOffSwitch.innerHTML = newState.lightOn ? "ON" : "OFF"

  //advanced control
  autoOffSwitch.checked = newState.doorAutoClose
  autoCloseTimerText.innerHTML = newState.doorAutoTime + "s"
  autoCloseRange.value = newState.doorAutoTime

  lightAutoCloseTimerText.innerHTML = newState.lightAutoOffTime + "s"
  autoLightRange.value = newState.lightAutoOffTime
  brightnessSliderText.innerHTML = newState.MaxBrightness + "%"
  brightnessRange.value = newState.MaxBrightness

}



function DoorOnOffButtonClicked() {
  garage.sendButton();
}
function DoorAutoCloseButtonClicked(){
  garage.setDoorAutoCloseEnable();
}


function autoOffSliderChange() {
    var slider = autoCloseRange.value
    garage.doorAutoTime(parseInt(slider))
}

function lightAutoOffSliderChange(){
  var slider = autoLightRange.value
  garage.setLightAutoOff(parseInt(slider))
}
function brightnessChange(){
  var slider = brightnessRange.value
  garage.setBrightness(parseInt(slider))
}
function lightButtonClicked(){
  garage.toggleLight();
}
