package logic;

public interface MessageChangeListener<T> {
    void onMessageChange(T newMessage);
}