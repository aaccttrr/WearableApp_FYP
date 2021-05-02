<template>
  <div class="container">
    <img src=./assets/handshake.png height = 150>
    <Header title = "Helping Hand"/>
    <HelpingHandController/>
    
  </div>
</template>

<script>
import Header from './components/Header'
import HelpingHandController from './components/HelpingHandController.vue'

export default {
  name: 'App',
  components: {
    //add components here
    Header,
    HelpingHandController,
  },

  data(){
    return {

    }
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
  max-width:inherit;
  margin-right: 150px;
  margin-left: 150px;
  overflow: auto;
  min-height:fit-content;
  border: 1px solid black;
  background-color: white;
  padding: 30px;
  border-radius: 5px;
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
