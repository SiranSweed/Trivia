#include "LoginRequestHandler.h"
#include "LoggedUser.hpp"
#include <exception>

/*
This is the class' CTOR - prepares a new LoginRequestHandler
object by initializing the handler factory.
Inputs: A handlerFactory to init with the m_handlerFactory field.
*/
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* handlerFactory) : m_handlerFactory(handlerFactory)
{
}

/*
This function validates the the code of the request is either login or signup
Inputs: struct of the request information.
Output: True if the request is one of both login ro sign up - false of anything else.
*/
bool LoginRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	if (reqInfo._id == LOGIN_REQ_CODE || reqInfo._id == SIGNUP_REQ_CODE || reqInfo._id == LOGOUT_REQ_CODE)
	{
		return true;
	}

	return false;
}

/*
This function handles the given request with an appropriate response.
Inputs: the sturct that has the request info.
Output: a struct of the proper request result based on the Id .
*/
RequestResult LoginRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	if (!this->isRequestRelevant(reqInfo))
	{
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Wrong request code" });
	}
	else // if it is a valid request code ( Relevant )
	{
		switch (reqInfo._id)
		{
		case RequestsCode::LOGIN_REQ_CODE:
			result = login(reqInfo);
			break;

		case RequestsCode::SIGNUP_REQ_CODE:
			result = signup(reqInfo);
			break;

		case RequestsCode::LOGOUT_REQ_CODE:
			result = logout(reqInfo);
			break;
		}
	}

	return result;
}

/*
This function calls the function from the login manager that is in
the handler factory, to operates a login with the request info.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult LoginRequestHandler::login(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		LoginRequest loginReq = JsonRequestPacketDeserializer::deserializeLoginRequest(reqInfo._buffer);
		this->m_handlerFactory->getLoginManager().login(loginReq.m_username, loginReq.m_password);

		// Handler to the next state. Login succeed --> moves on to a MenuRequestHandler
		result._newHandler = m_handlerFactory->createMenuRequestHandler(LoggedUser(loginReq.m_username));
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ 1 });
	}
	catch (std::exception& e)
	{
		// If an error occured, stay on the same state (LoginRequestHandler = this)
		result._newHandler = this;
		// Sends back an error response with the exception description as a message
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function calls the function from the login manager that is in
the handler factory, to operates a signup with the request info.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult LoginRequestHandler::signup(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		SignupRequest signupReq = JsonRequestPacketDeserializer::deserializeSignupRequest(reqInfo._buffer);
		this->m_handlerFactory->getLoginManager().signup(signupReq.m_username, signupReq.m_password, signupReq.m_email);

		// Handler to the next state. Signup succeed --> moves on to a MenuRequestHandler
		result._newHandler = m_handlerFactory->createMenuRequestHandler(LoggedUser(signupReq.m_username));
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(SignUpResponse{ 1 });
	}
	catch (std::exception& e)
	{
		// If an error occured, stay on the same state (RequestLoginHandler = this)
		result._newHandler = this;
		// Sends back an error response with the exception description as a message
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function operates a logout by sending back a serialized logout response - the
communicator class will check it and know to stop the communication and release resources.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult LoginRequestHandler::logout(const RequestInfo& reqInfo)
{
	RequestResult result;

	result._newHandler = nullptr;
	// Sends back an error response with the exception description as message
	result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "The user was not logged in" });

	return result;
}
