var myParticleAccessToken = "d6077f5cf83d13c901c689e030f3f28fb164839a"
var myDeviceId = "3a002f000c47363330353437"
var topic = "Assignment5"

//var doorState
var garageState = {
  0: "CLOSED",
  1: "CLOSING",
  2: "OPEN",
  3: "OPENING",
  4: "OPENING: interrupted",
  5: "CLOSING: interrupted",
  6: "OPENING: fault detected",
  7: "ClOSING: fault detected"
}
// NOTE: This callback is NOT in the light object to avoid mishandeling of "this"
// Consequently it explicitly changes properties of the global "light" object
// (This approach does not work well when there are multiple objects being interacted with)
function newGarageEvent(objectContainingData) {
  console.log("newEvent called");
//  console.dir(objectContainingData);
  if (garage.stateChangeListener == null) {
    console.log("State Change Listener is null")
    return;
  }
  var data = objectContainingData.data
  console.log(data)
  var json = JSON.parse(data)
  var doorState = garageState[parseInt(json.cur_state)]
  console.log(doorState)
  var state = {
    "cur_state": json.cur_state,
    "doorAutoClose": json.doorAutoClose,
    "doorAutoTime": json.doorAutoTime,
    "lightOn": json.lightOn,
    "lightAutoOffTime": json.lightAutoOffTime,
    "MaxBrightness": json.MaxBrightness
  }
  garage.stateChangeListener(state)
}

function callCloud(f, s) {
  function onSuccess(e) {
    console.log(f + "success")
  }

  function onFailure(e) {
    console.log(f + "fail")
    console.dir(e)
  }
  garage.particle.callFunction({ deviceId: myDeviceId, name: f, argument: s, auth: myParticleAccessToken }).then(onSuccess, onFailure)
}

var garage = {
  // Variable used to track listener function
  stateChangeListener: null,
  particle: null,

  setupChangeListener: function(GarageAppFunc) {
    this.stateChangeListener = GarageAppFunc
    this.setup()
  },

  setup: function() {
    // Create a particle object
    this.particle = new Particle();

    // Get ready to subscribe to the event stream
    function onSuccess(stream) {
      // DONE:  This will "subscribe' to the stream and get the state"
      console.log("getEventStream success")
      stream.on('event', newGarageEvent)
      //victoria
      callCloud("getState", "")
    };
    function onFailure(e) {
      console.log("getEventStream call failed")
      console.dir(e)
    };
    // Subscribe to the stream
    this.particle.getEventStream({deviceId: myDeviceId,name: topic, auth: myParticleAccessToken,}).then(onSuccess, onFailure)
  },
  sendButton: function(){
    callCloud("sendButton","")
  },
  setDoorAutoCloseEnable: function(){
    callCloud("setDoorAutoCloseEnable", "")
  },
  doorAutoTime: function(t){
    callCloud("doorAutoTime", String(t))
  },
  setLightAutoOff: function(t){
    callCloud("setLightAutoOff", String(t))
  },
  setBrightness: function(b){
    callCloud("setBrightness", String(b))
  },
  toggleLight: function(){
    callCloud("toggleLight","")
  }


  /*
  printGarageState: function(){
    return doorState;
  }*/
}
