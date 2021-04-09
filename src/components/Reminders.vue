<template>
    <div :key="reminder.id" v-for="reminder in reminders">
        <Reminder :reminder="reminder"/>
    </div>
</template>

<script>
import Reminder from './Reminder'

export default {
    name: 'Reminders',
    props: {
        reminders: Array
    },
    components:{
        Reminder,
    }
}

</script>