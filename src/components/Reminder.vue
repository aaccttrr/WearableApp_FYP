<template>
    <div>
        <h3>{{ reminder.text }}</h3>
        <p>{{ reminder.day }}</p>
    </div>
</template>

<script> 
export default {
    name: 'Reminder',
    props: {
        reminder: Object
    }
}
</script>