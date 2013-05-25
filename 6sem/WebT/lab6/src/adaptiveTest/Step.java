package adaptiveTest;

import java.io.IOException;
import java.util.List;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Step1
 */
@WebServlet("/Step")
public class Step extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Step() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		int complexity;
		
		if (request.getSession().getAttribute("step") == null ||
			     (int) request.getSession().getAttribute("step") > 4)
		{
			complexity = MathProblemGenerator.BASE_COMPLEXITY;
			request.getSession().setAttribute("complexity", complexity);
			request.getSession().setAttribute("step", 1);
		}
		
		if (request.getSession().getAttribute("complexity") != null)
			complexity = (int) request.getSession().getAttribute("complexity");
		else
			throw new ServletException("Wrong session");
		
		MathProblem mathProblem = MathProblemGenerator.generate(complexity);
		List<Long> answers = MathProblemGenerator.generateAnswers(mathProblem.getSolution());
		
		request.getSession().setAttribute("answer", mathProblem.getSolution());
		request.setAttribute("answers", answers);
		request.setAttribute("problem", mathProblem.getProblem());
		
		ServletContext sc = this.getServletContext();	
		RequestDispatcher rd = sc.getRequestDispatcher("/step.jsp");	
		rd.include(request, response);	
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		int complexity = (int) request.getSession().getAttribute("complexity");
		int step = (int) request.getSession().getAttribute("step");
		step++;
		if ( Long.parseLong(request.getParameter("ans")) == (long) request.getSession().getAttribute("answer"))
			complexity++;
		else
			complexity--;
		
		request.getSession().setAttribute("complexity", complexity);
		request.getSession().setAttribute("step", step);
		
		if (step > 4)
			response.sendRedirect("/lab6/Result");
		else
			response.sendRedirect(request.getRequestURI());
	}
}
