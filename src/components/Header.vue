<template>
    <header>
        <h1>{{title}}</h1>
    </header>
</template>

<script>

export default {
    name: 'Header',  
    props: {
        title: String,
    }
}
</script>

<style scoped>
    header{
        justify-content: space-between;
        align-items: center;
        margin-bottom: 20px;
    }
</style>