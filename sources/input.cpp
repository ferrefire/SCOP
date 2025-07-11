#include "input.hpp"

#include "manager.hpp"

void Input::AddKey(int keycode)
{
	keys[keycode] = KeyInfo{};
}

KeyInfo Input::GetKey(int keycode)
{
	if (!keys.contains(keycode)) AddKey(keycode);

	return (keys[keycode]);
}

void Input::UpdateKeys()
{
	for (auto& key : keys)
	{
		bool down = key.second.down;
		key.second.down = glfwGetKey(Manager::GetWindow().GetData(), key.first) == GLFW_PRESS;
		key.second.pressed = (!down && key.second.down);
		key.second.released = (down && !key.second.down);
	}
}

void Input::Frame()
{
	UpdateKeys();
}

std::map<int, KeyInfo> Input::keys;