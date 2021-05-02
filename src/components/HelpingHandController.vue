<template>
    <div>
        <!-- Buttons for handling connecting and disconnecting -->
        <div>
            <button
            @click="connectBluetooth()"
            type="button"
            class="btnGreen">Connect</button>

            <button v-if="helpingHand.isConnected"
            @click="disconnectBluetooth()"
            class="btnRed">Disconnect</button>

            <button v-if="fallDetected"
            @click="resetFallDetected()"
            class="btnRst">Reset Fall</button>
        </div>

        <!-- This only shows if a device is connected -->
        <div v-if="helpingHand.isConnected">
            <p>Device Connected!</p>
            <p>X: {{helpingHand.accelerometer.x}}   Y: {{helpingHand.accelerometer.y}}   Z: {{helpingHand.accelerometer.z}}</p>

            <div v-if="fallDetected">
                <button class="btnRed">FALL DETECTED !!!</button>
            </div>

        </div>

    </div>
</template>

<script>

export default {

    name: 'HelpingHandController',

    data(){
        return {

            fallDetected: false,

            //Object for storing data about the Helping Hand device
            helpingHand: {
                isConnected: false,
                device: null,
                greetingDataCharacteristic: null,
                accelerometerDataCharacteristic: null,
                accelerometer: {
                    x: 0,
                    y: 0,
                    z: 0
                }
            },

            helpingHandUuid: {
                // Services 
                accelService: ['91c39bfd-1921-4111-9670-c03fc51eb856', 'Accelerometer Service'],
                
                //greetingService: ['c8f38c8d-a05e-4c34-aa66-3f47cd41f6c9', 'Greeting Service'], (used for testing)

                // Characteristics
                accelCharX: ['2a0019db-2b41-49f7-b3a7-d6deca619810', 'Acceleration X Characteristic'],
                accelCharY: ['4e41b6eb-8acc-44d7-961b-9b2706764b49', 'Acceleration Y Characteristic'],
                accelCharZ: ['37855a77-602f-4177-b916-c103a1723f5d', 'Acceleration Z Characteristic'],

                //greetingCharacteristic: ['b292c8b1-a6a0-4ef3-92ac-803921d82a3c', 'Greeting Characteristic'], (used for testing)
            }
        }
    },


    

    methods:{

        //Converts data into standard numbers and stores it in the device object
        xDataChanged(event){
            this.helpingHand.accelerometer.x = event.target.value.getInt16(0, true);
            console.log({x: this.helpingHand.accelerometer.x});

        },

        yDataChanged(event){
            this.helpingHand.accelerometer.y = event.target.value.getInt16(0, true);
            console.log({y: this.helpingHand.accelerometer.y});

        },

        zDataChanged(event){
            this.helpingHand.accelerometer.z = event.target.value.getInt16(0, true);
            console.log({z: this.helpingHand.accelerometer.z});

            if(this.helpingHand.accelerometer.z > 300){
                console.log('Fall Detected');
                this.fallDetected = true;
            }

        },

        //Connect to device using a bluetooth navigator
        async connectBluetooth(){

            // Creates a list of optional service UUIDs to allow access
            let optionalServicesList = [
                //this.helpingHandUuid.greetingService[0],
                this.helpingHandUuid.accelService[0]
            ];

            if(!navigator.bluetooth){
                console.log('Bluetooth Adapter Unavailable')
            }else{
                navigator.bluetooth.requestDevice({
                    filters: [{namePrefix: 'HelpingHand'}],
                    optionalServices: optionalServicesList
                })
                .then((device)=>{
                    // On successful connection
                    console.log('Connection made');
                    // Stores the helpingHand object in the variable "device"
                    this.helpingHand.device = device;
                    console.log("Device Name: "+ device.name)
                    //runs on disconnect
                    //device.addEventListener('Gatt Server Disconnected', this.ifDisconnected);
                    return device.gatt.connect();
                })
                .then((server) =>{
                    this.helpingHand.isConnected = true;
                    console.log('Getting Accelerometer Service');
                    return server.getPrimaryService(this.helpingHandUuid.accelService[0]);
                })
                .then((service) =>{
                    console.log('Getting Accelerometer Characteristics');
                    //Gets the X axis data
                    service
                        .getCharacteristic(this.helpingHandUuid.accelCharX[0])
                        .then((dataX) => {
                            this.helpingHand.accelCharX = dataX;
                            console.log(dataX);
                            return dataX
                                .startNotifications()
                                .then(()=>{
                                    dataX.addEventListener('characteristicvaluechanged', this.xDataChanged);
                                    console.log(this.helpingHand.accelerometer.x);
                                })
                                .catch((error)=>{
                                    console.error(error);
                                })
                        })
                        .catch((error)=>{
                            console.error(error);
                        })
                        //Gets the Y axis data
                    service
                        .getCharacteristic(this.helpingHandUuid.accelCharY[0])
                        .then((dataY) => {
                            this.helpingHand.accelCharY = dataY;
                            console.log(dataY);
                            return dataY
                                .startNotifications()
                                .then(()=>{
                                    dataY.addEventListener('characteristicvaluechanged', this.yDataChanged);
                                    console.log(this.helpingHand.accelerometer.y);
                                })
                                .catch((error)=>{
                                    console.error(error);
                                })
                        })
                        .catch((error)=>{
                            console.error(error);
                        })
                        //Gets the Z axis data
                    service
                        .getCharacteristic(this.helpingHandUuid.accelCharZ[0])
                        .then((dataZ) => {
                            this.helpingHand.accelCharZ = dataZ;
                            console.log(dataZ);
                            return dataZ
                                .startNotifications()
                                .then(()=>{
                                    dataZ.addEventListener('characteristicvaluechanged', this.zDataChanged);
                                    console.log(this.helpingHand.accelerometer.z);
                                })
                                .catch((error)=>{
                                    console.error(error);
                                })
                        })
                        .catch((error)=>{
                            console.error(error);
                        })
                })
                .catch((error)=>{
                    console.error(error);
                })
            }
        },


        disconnectBluetooth(){
            console.log('Disconnecting')
            //Check if a device exists before trying to disconnect
            if(!this.helpingHand.device){
                console.error('No device connected')
            }else{
                //Check if stored device is connected
                if(this.helpingHand.device.gatt.connected){
                    //Disconnect
                    this.helpingHand.device.gatt.disconnect();

                    //Resets all variables for the next connection
                    this.helpingHand.isConnected = false;
                    this.helpingHand.accelerometer.x = 0;
                    this.helpingHand.accelerometer.y = 0;
                    this.helpingHand.accelerometer.z = 0;
                    this.fallDetected = false;
                    console.log('Device Disconnected')
                }else{
                    console.error('No device connected')
                }
            }
        },

        //for handling the device disconnecting
        ifDisconnected(){
            console.log("Device BLE Disconnected");
            this.helpingHand.isConnected = false;
        },

        resetFallDetected(){
            this.fallDetected = false;
        }

    }

}
</script>

<style scoped>

.btnGreen {
  display: inline-block;
  color: white;
  background: green;
  border: none;
  padding: 10px 20px;
  margin: 5px;
  border-radius: 10px;
  cursor: pointer;
  text-decoration: none;
  font-size: 15px;
  font-family: inherit;
}

.btnRed {
  display: inline-block;
  color: white;
  background: darkred;
  border:black;
  padding: 10px 20px;
  margin: 5px;
  border-radius: 10px;
  cursor: pointer;
  text-decoration: none;
  font-size: 15px;
  font-family: inherit;
}

.btnRst {
  display: inline-block;
  color: white;
  background: black;
  border: none;
  padding: 10px 20px;
  margin: 5px;
  border-radius: 10px;
  cursor: pointer;
  text-decoration: none;
  font-size: 15px;
  font-family: inherit;
}
</style>