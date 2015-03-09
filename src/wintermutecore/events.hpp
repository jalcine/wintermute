/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef WINTERMUTE_CORE_EVENTS_HPP
# define WINTERMUTE_CORE_EVENTS_HPP

#include <wintermutecore/globals.hpp>
#include <map>
#include <list>

#define W_EVENT_POLLED    "core.events.poll"
#define W_EVENT_TIMEOUT   "core.events.timeout"
#define W_EVENT_SIGNAL    "core.events.signal"

namespace Wintermute
{
namespace Events
{
class EventPrivate;
class LoopPrivate;
class ListenerPrivate;
class EmitterPrivate;
class PollerPrivate;
class TimerPrivate;
class SignalHandlerPrivate;

/**
 * Serves as a basis for event loops in Wintermute.
 * The Loop object represents the event loop that's provided by [libuv][].
 * It handles the memory management for the loop and allows for it to be used
 * without too much concern.
 *
 * @note This object is NOT threadsafe since the underlying data type,
 * uv_loop_t isn't.
 *
 * [libuv]: https://github.com/libuv/libuv
 */
// TODO: Add support for UV_RUN_ONCE.
// TODO: Add support for UV_RUN_NOWAIT.
class Loop : W_DEF_SHAREABLE(Loop)
{
protected:
  W_DEF_PRIVATE(Loop)
public:
  friend class Poller;
  friend class Timer;
  friend class SignalHandlerPrivate;
  W_DECL_PTR_TYPE(Loop)

  /**
   * Obtains a Loop object that represents the default loop.
   * @return A pointer to a Loop object.
   * Allocates a new object that allows you to work with the default event
   * loop.
   * @note This isn't a statically available object; it's a brand new one each
   * time.
   */
  static Ptr primary();

  /**
   * Begins running the event loop.
   * @return TRUE if the loop was started successfully.
   * @return FALSE if the loop could not be started.
   */
  bool run();

  /**
   * Creates a new Loop.
   * @param[in] primary Flag to determine if we're creating a new Loop.
   */
  explicit Loop(const bool primary = true);

  ///< Destructor.
  virtual ~Loop();

  /**
   * Determines whether or not this Loop is active.
   * @return TRUE if the loop is running, FALSE otherwise.
   */
  bool isRunning() const;

  /**
   * Determines whether or not this loop represents the default loop.
   * @return TRUE if the loop is the default loop, FALSE otherwise.
   */
  bool isPrimary() const;

  /**
   * Stops this Loop.
   * @return TRUE if successful, FALSE otherwise.
   */
  bool stop();
};

/**
 * Represents an Event passed from a Emittable object.
 * This serves as a base class for sharing information to events. Every event
 * in Wintermute should base itself upon this class in an attempt to provide a
 * solid hierarchy for event handling.
 */
class Event : W_DEF_SHAREABLE(Event)
{
  W_DEF_PRIVATE(Event);
public:
  W_DECL_PTR_TYPE(Event)

  /**
   * Implements a new Event object.
   * @param[in] event The name of the Event.
   */
  explicit Event(const string& event);

  /**
   * Destroys this Event object.
   */
  virtual ~Event();

  /**
   * Obtains the name of this Event.
   * @return The name of this event.
   */
  string name() const;
};

/**
 * Represents a callback to be invoked for a provided Event.
 * @sa Emitter
 * @sa Event
 *
 * Listener objects capture a valid form of a function and store it for
 * later invocation for event raising. It prohibits the use of empty callbacks
 * so that one can use a Events::Emitter knowing a function is being hit for
 * each listener.
 */
class Listener : W_DEF_SHAREABLE(Listener)
{
  W_DEF_PRIVATE(Listener);
public:
  W_DECL_PTR_TYPE(Listener)
  /**
   * The known frequenices at which a Listener should be invoked.
   */
  enum Frequency
  {
    FrequencyUndefined = 0x000, ///< No invocation should occur.
    FrequencyEvery     = 0x100, ///< Invocation on every emission.
    FrequencyOnce      = 0x200  ///< Invocation only once, remove after.
  };

  ///< Represents the signature of the function to wrap.
  ///< @sa Wintermute::Events::Listener::Callback
  typedef void(CallbackSignature)(const Event::Ptr&);

  ///< Represents a callable object type for Listeners.
  ///< @sa Wintermute::Events::Listener::CallbackSignature
  typedef std::function<CallbackSignature> Callback;

  ///< Represents a list of Listener pointers.
  typedef std::list<Listener::Ptr> List;

  ///< Represents a 1-to-n mapping for Listener pointers.
  typedef std::multimap<string, Listener::Ptr> Map;

  /**
   * Constructor for the Listener.
   * @param[in] callback The callback to use for the Listener.
   * @throw std::invalid_argument Thrown if the callback provided isn't valid.
   */
  explicit Listener(Callback callback) throw (std::invalid_argument);

  ///< Destructor.
  virtual ~Listener();

  /**
   * Invokes the stored Callback function with the provided Event pointer.
   * @param[in] Event::Ptr The Event that's to be used by this Listener.
   * @throw std::invalid_argument Thrown if the Event is null.
   */
  void invoke(const Event::Ptr& event) throw (std::invalid_argument);

  ///< The frequency at which this Listener should be invoked.
  Frequency frequency;
};

/**
 * Serves as a store for events that can be invoked.
 * @sa Wintermute::Events::Event
 * @sa Wintermute::Events::Listener
 * Emitter objects take a collection of Listener objects for a list of known,
 * named Event objects and stores them for later invocation when deemed
 * appropriately calling in the loop.
 */
class Emitter : W_DEF_SHAREABLE(Emitter)
{
private:
  W_DEF_PRIVATE(Emitter)
public:
  W_DECL_PTR_TYPE(Emitter)
  /**
   * Creates a new Emitter.
   * @param[in] loop {optional} The Loop pointer to use.
   */
  explicit Emitter(const Loop::Ptr& loop = Loop::primary());

  ///< Destructor.
  virtual ~Emitter();

  /**
   * Listens for the named event to  invoke the provided callback.
   * @param[in] name The name of the Event to be invoked.
   * @param[in] func The Callback to be used for later invocation.
   * @param[in] freq The frequency at which this could be raised.
   * @return A pointer to the crafted Listener pointer.
   */
  Listener::Ptr listen(const string & name, Listener::Callback func, const Listener::Frequency& freq = Listener::FrequencyEvery);

  /**
   * Listens for the named event to  invoke the provided callback.
   * @param[in] name The name of the Event to be invoked.
   * @param[in] listener The Listener to add to this Emitter.
   * @return A pointer to the crafted Listener pointer.
   */
  Listener::Ptr listen(const string & name, Listener::Ptr& listener);

  /**
   * Removes the provided Listener from this Emitter.
   * @param[in] listener The Listener to remove.
   * @return TRUE if the listener was successfully removed.
   * @return FALSE if the listener was not removed.
   */
  bool stopListening(const Listener::Ptr & listener);

  /**
   * Obtains a list of Listener objects for the particular Event.
   * @param[in] eventName The event in question.
   * @return A list of Listener objects if the event's been listened to.
   */
  Listener::List listeners(const string & eventName) const;

  /**
   * Invokes the Listener objects for the provided Event.
   * @param[in] event The Event being emitted to Listener objects.
   */
  void emit(const Event::Ptr & event);

  /**
   * Obtains the Loop used by this Emitter.
   * @return A pointer to a Loop object.
   */
  Loop::Ptr loop() const;
};

/**
 * Helper class that allows for "plug-n-play" support for the event system.
 * @sa Wintermute::Events::Emitter
 */
class Emittable
{
public:
  /**
   * Returns the underlying Emitter object used by this Emittable.
   * @return A pointer to a Emitter object.
   */
  virtual Emitter::Ptr emitter() const = 0;

  ///< @sa Wintermute::Events::Emitter::listen
  Listener::Ptr listenForEvent(const string & name, Listener::Callback func,
    const Listener::Frequency& freq = Listener::FrequencyEvery)
  {
    return emitter()->listen(name, func, freq);
  }

  ///< @sa Wintermute::Events::Emitter::stopListening
  bool removeEventListener(const Listener::Ptr& listener)
  {
    return emitter()->stopListening(listener);
  }

  ///< @sa Wintermute::Events::Emitter::listeners
  Listener::List eventListeners(const string & eventName) const
  {
    return emitter()->listeners(eventName);
  }

  ///< @sa Wintermute::Events::Emitter::emit
  void emitEvent(const Event::Ptr & event)
  {
    emitter()->emit(event);
  }
};

/**
 * Represents a polling object.
 * @sa PollEvent
 * Poller allows one to poll on file descriptors friendly to [libuv][poll]. It
 * emits the PollEvent when a new event has been triggered by the provided
 * file descriptor.
 *
 * [poll]: http://libuv.readthedocs.org/en/latest/handle.html#c.uv_fileno
 */
class Poller : public Emittable
#ifndef DOXYGEN_SKIP
  , W_DEF_SHAREABLE(Poller)
#endif
{
    W_DEF_PRIVATE(Poller);
  public:
    W_DECL_PTR_TYPE(Poller)

    /**
     * Determines what kind of listening on the file descriptor you'd want.
     */
    enum PollDirection
    {
      PollReadable = 1, ///< Emit events when we can read.
      PollWritable = 2  ///< Emit events when we can write.
    } ;

    ///< Platform dependent implementation of the file descriptor type.
    typedef int FileDescriptor;

    ///< The Emitter used by this Poller.
    ///< @sa Emittable
    virtual Emitter::Ptr emitter() const;

    /**
     * Builds a new Poller object.
     * @param[in] fd The file descriptor to listen on.
     * @param[in] poll The polling directions to listen for.
     * @param[in] loopPtr The Loop on which to work on.
     */
    explicit Poller(const FileDescriptor& fd,
      const PollDirection& poll = PollReadable,
      const Loop::Ptr& loopPtr = Loop::primary());

    ///< Destructor.
    virtual ~Poller();

    ///< The file descriptor we're working with.
    FileDescriptor fd() const;

    ///< The Loop this Poller works on.
    Loop::Ptr loop() const;

    ///< The directions of polling this Poller listens for.
    PollDirection direction() const;

    ///< Starts the Poller.
    bool start();

    ///< Stops the Poller.
    bool stop();

    ///< Determines if the Poller is active.
    bool isActive() const;
};

/**
 * Represents a polling event emitted by a Poller object.
 * @sa Poller
 */
class PollEvent : public Event
{
  public:
    /**
     * Creates a new PollEvent for the provided Poller.
     * @param thePoller The poller to use.
     */
    PollEvent(Poller::Ptr& thePoller) :
      Event(W_EVENT_POLLED),
      poller(thePoller) { }

    ///< Destructor.
    virtual ~PollEvent() { }

    ///< The Poller that emitted this Event.
    Poller::Ptr poller;
};

/**
 * Represents a time-based emission of events.
 * With a provided interval or timeout, Timer allows one to periodically emit
 * events.
 */
class Timer : public Emittable
#ifndef DOXYGEN_SKIP
  , W_DEF_SHAREABLE(Timer)
#endif
{
    W_DEF_PRIVATE(Timer);
  public:
    W_DECL_PTR_TYPE(Timer)

    ///< The Emitter for this Timer.
    virtual Emitter::Ptr emitter() const;

    /**
     * Default constructor.
     * @param[in] loopPtr The Loop this Timer works on.
     */
    explicit Timer(const Loop::Ptr& loopPtr = Loop::primary());

    ///< Destructor.
    virtual ~Timer();

    /**
     * Starts this Timer, after a provided timeout.
     * @param[in] uint64_t The timeout in which to emit TimerEvent objects.
     * @return TRUE if successful.
     */
    bool start(const uint64_t timeout = 0);

    ///< Stops this Timer.
    ///< @return TRUE if successful.
    bool stop();

    ///< Determines if this Timer is actively running.
    ///< @return TRUE if successful.
    bool isActive() const;

    ///< Obtains the interval at which this Timer object goes off.
    uint64_t interval() const;

    /**
     * Sets the interval at which the Timer can go off.
     * @param[in] uint64_t The interval for this Timer.
     */
    void setInterval(const uint64_t newInterval);
};

/**
 * An Event object to be risen when a Timer reaches its timeout/interval.
 */
class TimerEvent : public Event
{
  public:
    ///< Constructor.
    TimerEvent(Timer::Ptr theTimer) :
      Event(W_EVENT_TIMEOUT),
      timer(theTimer) { }

    ///< Destructor.
    virtual ~TimerEvent() { }

    ///< A pointer to the Timer in question.
    Timer::Ptr timer;
};

class SignalHandler : public Emittable
#ifndef DOXYGEN_SKIP
  , W_DEF_SHAREABLE(SignalHandler)
#endif
{
  private:
    W_DEF_PRIVATE(SignalHandler);
  public:
    W_DECL_PTR_TYPE(SignalHandler)

    explicit SignalHandler(const int& signalNumber);
    virtual ~SignalHandler();

    ///< The signal handler's emitter.
    virtual Emitter::Ptr emitter() const;

    ///< The signal this handler is listening to.
    int signal() const;

    SignalHandler::Ptr signalHandler;
};

class SignalEvent : public Event
{
  public:
    explicit SignalEvent(const SignalHandler::Ptr& handler) :
      Event(W_EVENT_SIGNAL), signalHandler(handler) {}

    virtual ~SignalEvent() { }

    SignalHandler::Ptr signalHandler;
};

}
}
#endif
