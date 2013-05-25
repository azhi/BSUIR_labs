<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Login</title>
<style>
  #main {
    width: 400px;
    margin-left: auto;
    margin-right: auto;
  }
  
  div.input_wrapper {
    position: relative;
    margin: 10px 0;
  }
  
  form input[type="text"], form input[type="password"] {
    position: absolute;
    right: 0;
  }
  
  form input[type="submit"] {
    padding: 5px 30px;
  }
  
  p.error {
    color: #f00;
    font-size: 150%;
  }
  
  p.success {
    color: #0f4;
    font-size: 150%;
  }
</style>
</head>
<body>
  <div id="main">
    <% if (request.getAttribute("e_msg") != null) { %>
      <p class="error"><%= request.getAttribute("e_msg") %></p>
    <% } %>
      
    <% if (request.getAttribute("s_msg") != null) { %>
      <p class="success"><%= request.getAttribute("s_msg") %></p>
    <% } %>
	<form action="<%= request.getRequestURI() %>" method="post">
	  <div class="input_wrapper"><label> Nickname: <input type="text" name="nickname" /></label></div>
	  <div class="input_wrapper"><label> Password: <input type="password" name="password" /></label></div>
	  <input type="submit" value="Login" />
	</form>
</body>
</html>