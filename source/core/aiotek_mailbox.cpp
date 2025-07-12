#include "aiotek_mailbox.hpp"

namespace AIOTEK {

void Mailbox::send(const MailboxEnvelope& env)
{
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(env);
    cond_.notify_one();
}

MailboxEnvelope Mailbox::receive()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] { return !queue_.empty(); });
    auto env = queue_.front();
    queue_.pop();
    return env;
}

std::optional<MailboxEnvelope> Mailbox::try_receive()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty())
        return std::nullopt;
    auto env = queue_.front();
    queue_.pop();
    return env;
}

Mailbox g_mailbox;
} // namespace AIOTEK
