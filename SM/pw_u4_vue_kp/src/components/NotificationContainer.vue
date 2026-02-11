<template>
  <div class="notification-container">
    <div
      v-for="notification in notifications"
      :key="notification.id"
      :class="['notification-card', notification.type]"
    >
      <p>{{ notification.message }}</p>
    </div>
  </div>
</template>

<script>
export default {
  name: "NotificationContainer",
  props: {
    notifications: {
      type: Array,
      required: true,
    },
  },
  emits: ["remove"],
};
</script>

<style scoped>
.notification-container {
  margin: 20px auto;
  max-width: 800px;
  padding: 0 20px;
  display: flex;
  flex-direction: column;
  gap: 15px;
  z-index: 1000;
}

.notification-card {
  width: 100%;
  padding: 20px;
  border-radius: 8px;
  position: relative;
  box-sizing: border-box;
}

p {
  margin: 0;
  font-size: 16px;
  font-weight: 500;
  line-height: 1.5;
  color: #333;
}

/* Card Types */
.success {
  background-color: #d4edda;
  border: 1px solid #c3e6cb;
  color: #155724;
}
.success p {
  color: #155724;
}

.error {
  background-color: #f8d7da;
  border: 1px solid #f5c6cb;
  color: #721c24;
}
.error p {
  color: #721c24;
}

.info {
  background-color: #fff3cd;
  border: 1px solid #ffeeba;
  color: #856404;
}
.info p {
  color: #856404;
}
</style>
