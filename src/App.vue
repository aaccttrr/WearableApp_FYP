<template>
  <div class="container">
    <Header title = "Medical Scheduler"/>
    <Reminders :reminders="reminders" />
  </div>
</template>

<script>
import Header from './components/Header'
import Reminders from './components/Reminders'

export default {
  name: 'App',
  components: {
    //add components here
    Header,
    Reminders
  },

  data(){
    return {
      reminders: []
    }
  },
  
  created(){
    this.reminders = [
      {
        id:1,
        text: 'Appointment',
        day: 'April 1st at 4:00 PM',
        reminder: true,
      },
      {
        id:2,
        text: 'Exercise',
        day: 'May 21st at 1:00 PM',
        reminder: true,
      },

    ]
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

.container {
  max-width: 500px;
  margin:30pc auto;
  overflow: auto;
  min-height: 500px;
  border: 1px solid black;
  padding: 30px;
  border-radius: 5px;
}

.btn {
  display: inline-block;
  background: #000;
  color: #fff;
  border: none;
  padding: 10px 20px;
  margin: 5px;
  border-radius: 5px;
  cursor: pointer;
  text-decoration: none;
  font-size: 15px;
  font-family: inherit;
}

.btn:focus {
  outline: none;
}

.btn:active {
  transform: scale(0.98);
}

.btn-block {
  display: block;
  width: 100%;
}
</style>
