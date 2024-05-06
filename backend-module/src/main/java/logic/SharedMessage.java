package logic;

/*
* This design pattern is known as the Observer pattern.
* It allows objects (observers) to be notified when the state of another object (subject) changes.
*  In this case, the SharedMessage class is the subject, and the MessageChangeListener objects are the observers.
*/
import java.util.ArrayList;
import java.util.List;

public class SharedMessage<T> {
    private T message;
    private final List<MessageChangeListener<T>> listeners = new ArrayList<>();

    public SharedMessage() {
    }
    public SharedMessage(T aDefault) {
        this.message = aDefault;
    }

    public synchronized T getMessage() {
        return message;
    }

    public synchronized void setMessage(T message) {
        this.message = message;
        notifyMessageChange(message);
        notifyAll();
    }

    public void addSpeedChangeListener(MessageChangeListener<T> listener) {
        listeners.add(listener);
    }

    public void removeSpeedChangeListener(MessageChangeListener<T> listener) {
        listeners.remove(listener);
    }

    private void notifyMessageChange(T newMessage) {
        for (MessageChangeListener<T> listener : listeners) {
            listener.onMessageChange(newMessage);
        }
    }
}