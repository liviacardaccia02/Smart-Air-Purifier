package logic;

public class SharedMessage<T> {
    private T message;

    public SharedMessage(T defaultMessage) {
        this.message = defaultMessage;
    }

    public synchronized T getMessage() {
        return message;
    }

    public synchronized void setMessage(T message) {
        this.message = message;
    }
}
