namespace chat
{
    enum Results
    {
        login_success,
        login_cancel,
        register_success,
        register_cancel,
        yes,
        no,
        app_exit,
        back,
        empty,
        chat,
        admin,
        registration,
        login,
        send_message,
        chat_options,
        private_chat,
        public_chat,
    };
};

namespace page
{
    enum PaginationMode
    {
        page,
        last_page,
        message,
        msg_per_page,
        close_options,
    };
}
