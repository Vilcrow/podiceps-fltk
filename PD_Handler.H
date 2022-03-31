#ifndef PD_HANDLER_H_SENTRY
#define PD_HANDLER_H_SENTRY

class PD_InputHandler {
public:
	virtual void Processing(const char*);
};

class PD_OptionsHandler : public PD_InputHandler {
public:
	virtual void Processing(const char*);
};

#endif
