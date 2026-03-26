import { ref, type Ref } from "vue";
import type { User } from "../models/user";

export var userId = ref(0);
export var userInfo: Ref<User> = ref({});