from cs50 import SQL
from flask import Flask, flash, redirect, render_template
from flask import request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = (
            "no-cache, no-store, must-revalidate"
        )
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

dberror = "database error"


@app.route("/")
@login_required
def index():
    """ Shows portfolio. """

    id = session.get("user_id")

    # check the available cash
    rows = db.execute(
        "SELECT cash FROM users WHERE id = :id",
        id=id
    )
    cash = rows[0]["cash"]
    total = cash

    # get available stocks (symbol and and shares)
    stocks = db.execute(
        "SELECT * from :portfolio",
        portfolio="portfolio_" + str(id)
    )

    # update with current prices and calculate sums
    for stock in stocks:
        lkp = lookup(stock["symbol"])
        stock["name"] = lkp["name"]
        stock["curr_price"] = usd(lkp["price"])
        stock["total"] = usd(lkp["price"]*stock["shares"])
        total = total + lkp["price"]*stock["shares"]

    # call the table template
    return render_template("index.html",
                           total=usd(total),
                           stocks=stocks,
                           cash=usd(cash))


@app.route("/buy_sell", methods=["POST"])
@login_required
def buy_sell():
    """ Decide whether the user wants to buy or sell and redirect """

    # http://stackoverflow.com/q/354038/6049386
    try:
        shares = int(request.form.get("shares"))
    except ValueError:
        return apology("invalid shares")

    symbol = request.form.get("symbol")

    # http://stackoverflow.com/a/15480983/6049386
    if shares == 0:
        return apology("invalid shares")
    elif shares > 0:
        return redirect(url_for("buy"), code=307)
    else:
        session["type"] = "redirect"
        return redirect(url_for("sell"), code=307)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        req_symbol = request.form.get("symbol")
        req_shares = request.form.get("shares")

        # ensure something was submitted
        if not req_symbol:
            return apology("missing symbol")
        if not req_shares:
            return apology("missing shares")

        # query
        stock = lookup(req_symbol)

        # ensure stock exists
        if not stock:
            return apology("invalid symbol")

        # this checks for non-negative int
        if not req_shares.strip("+").isdigit():
            return apology("invalid shares")
        shares = int(req_shares)
        if shares == 0:
            return apology("invalid shares")

        # set some variables
        name = stock["name"]
        price = float(stock["price"])
        symbol = stock["symbol"]
        transaction = shares*price
        id = session.get("user_id")

        # check the available cash
        rows = db.execute(
            "SELECT cash FROM users WHERE id = :id",
            id=id
        )
        cash = rows[0]["cash"]
        if cash < transaction:
            return apology("can't afford")

        # add the stock to the user's portfolio
        # try to insert as new
        sql_query = db.execute(
            "INSERT INTO :portfolio (symbol, shares) " +
            "VALUES (:symbol, :shares)",
            portfolio="portfolio_" + str(id), symbol=symbol, shares=shares
        )

        # can not insert - field exists - then update it
        if not sql_query:
            sql_query = db.execute(
                "UPDATE :portfolio SET shares = shares + :shares " +
                "WHERE symbol = :symbol",
                portfolio="portfolio_" + str(id),
                shares=shares,
                symbol=symbol
            )
        # error - neither update nor insert worked
        if not sql_query:
            return apology(dberror)

        # update the user's cash
        sql_query = db.execute(
            "UPDATE users SET cash = cash - :transaction WHERE id = :id",
            transaction=transaction,
            id=id
        )
        if not sql_query:
            return apology(dberror)

        # keep track of the purchase
        sql_query = db.execute(
            "INSERT INTO :history (symbol, shares, price) " +
            "VALUES (:symbol, :shares, :price)",
            history="history_" + str(id),
            symbol=symbol,
            shares=shares,
            price=usd(price)
        )
        if not sql_query:
            return apology(dberror)

        flash("Bought!")
        return redirect(url_for("index"))

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    id = session.get("user_id")

    # read the history
    stocks = db.execute(
        "SELECT * FROM :history",
        history="history_" + str(id))

    return render_template("history.html",
                           stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST
    #   (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = :username",
            username=request.form.get("username")
        )
        if not rows:
            return apology("invalid username and/or password")

        # ensure password is correct
        password = pwd_context.verify(request.form.get("password"),
                                      rows[0]["hash"])
        if not password:
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure something was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol")

        # query
        stock = lookup(request.form.get("symbol"))

        # ensure stock exists
        if not stock:
            return apology("invalid symbol")

        return render_template("quoted.html",
                               name=stock["name"],
                               symbol=stock["symbol"],
                               price=stock["price"])

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        name = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # ensure username was submitted
        if not name:
            return apology("must provide username")

        # ensure password was submitted
        elif not password:
            return apology("must provide password")

        # ensure password confirmation matched
        elif not (password == confirmation):
            return apology("passwords don't match")

        # query database for username
        sql_query_key = db.execute(
            "INSERT INTO users (username, hash) VALUES (:username, :hash)",
            username=name,
            hash=pwd_context.encrypt(password)
        )

        # check if INSERT is succesfull
        if not sql_query_key:
            return apology("the username " + name + " is taken")

        # log the user automatically
        id = sql_query_key
        session["user_id"] = id

        # create the user specific tables
        sql_query = db.execute(
            "CREATE TABLE :table (" +
            "symbol TEXT PRIMARY KEY NOT NULL, " +
            "shares INTEGER NOT NULL)",
            table="portfolio_" + str(id))
        if not sql_query:
            return apology(dberror)

        sql_query = db.execute(
            "CREATE TABLE :table (" +
            "symbol TEXT NOT NULL, " +
            "shares INTEGER NOT NULL," +
            "price TEXT NOT NULL, " +
            "time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP)",
            table="history_" + str(id))
        if not sql_query:
            return apology(dberror)

        # redirect user to home page
        flash("Registered!")
        return redirect(url_for("index"))

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        req_symbol = request.form.get("symbol")
        req_shares = request.form.get("shares")

        # ensure something was submitted
        if not req_symbol:
            return apology("missing symbol")
        if not req_shares:
            return apology("missing shares")

        symbol = req_symbol.upper()

        id = session.get("user_id")
        pf = db.execute(
            "SELECT * FROM :portfolio WHERE symbol = :symbol",
            portfolio="portfolio_" + str(id),
            symbol=symbol
        )

        # ensure stock exists in portfolio
        if not pf:
            return apology("invalid symbol")

        # check the shares
        # http://stackoverflow.com/q/354038/6049386
        try:
            shares = int(req_shares)
        except ValueError:
            return apology("invalid shares")

        # handle the negative value when redirected from buy_sell
        if session.get("type") == "redirect":
            session["type"] = ""
            shares = -shares

        if shares <= 0 or pf[0]["shares"] < shares:
            return apology("invalid shares")

        stock = lookup(req_symbol)
        transaction = shares*stock["price"]

        # update the user's cash
        sql_query = db.execute(
            "UPDATE users SET cash = cash + :transaction WHERE id = :id",
            transaction=transaction,
            id=id
        )

        # update the portfolio:
        # no shares left - delete the entry
        if pf[0]["shares"] == shares:
            sql_query = db.execute(
                "DELETE FROM :portfolio WHERE symbol = :symbol",
                portfolio="portfolio_" + str(id),
                symbol=symbol
            )
        # or just update it
        else:
            sql_query = db.execute(
                "UPDATE :portfolio SET shares = shares - :shares " +
                "WHERE symbol = :symbol",
                portfolio="portfolio_" + str(id),
                shares=shares,
                symbol=symbol
            )

        # keep track of the purchase
        sql_query = db.execute(
            "INSERT INTO :history (symbol, shares, price) " +
            "VALUES (:symbol, :shares, :price)",
            history="history_" + str(id),
            symbol=symbol,
            shares=-(shares),
            price=usd(stock["price"])
        )

        flash("Sold!")
        return redirect(url_for("index"))

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")


@app.route("/pwdchg", methods=["GET", "POST"])
@login_required
def pwdchg():
    """Change Password."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        oldpwd = request.form.get("oldpwd")
        newpwd = request.form.get("newpwd")
        confirmation = request.form.get("confirmation")

        # ensure everything was submitted
        if not oldpwd or not newpwd or not confirmation:
            return apology("all fields required")

        # ensure the new password confirmation matched
        elif not (newpwd == confirmation):
            return apology("passwords don't match")

        id = session.get("user_id")

        # check the old password
        sql_query_hash = db.execute(
            "SELECT hash FROM users WHERE id=:id",
            id=id
        )
        password = pwd_context.verify(oldpwd, sql_query_hash[0]["hash"])
        if not password:
            return apology("wrong password")

        # UPDATE the password
        sql_query = db.execute(
            "UPDATE users SET hash = :hash WHERE id = :id",
            hash=pwd_context.encrypt(newpwd),
            id=id
        )

        # redirect user to home page
        flash("Password changed!")
        return redirect(url_for("index"))

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("pwdchg.html")


@app.route("/funds", methods=["GET", "POST"])
@login_required
def funds():
    """ Add/Withdraw Funds."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        id = session.get("user_id")

        amount = request.form.get("funds")
        # http://stackoverflow.com/q/354038/6049386
        try:
            funds = float(amount)
        except ValueError:
            return apology("invalid amount")

        # for withdrawals checks if valid
        if funds < 0:
            rows = db.execute(
                "SELECT cash FROM users WHERE id = :id",
                id=id
            )
            cash = rows[0]["cash"]
            if cash < abs(funds):
                return apology("can't afford")

        # update the user's cash
        sql_query = db.execute(
            "UPDATE users SET cash = cash + :funds WHERE id = :id",
            funds=funds,
            id=id
        )

        # keep track of the transaction
        if funds < 0:
            type = "withdrawal"
            pminus = -1
        else:
            type = "--deposit--"
            pminus = 1
        sql_query = db.execute(
            "INSERT INTO :history (symbol, shares, price) " +
            "VALUES (:type, :pminus, :price)",
            history="history_" + str(id),
            type="--" + type + "--",
            pminus=pminus,
            price=usd(abs(funds))
        )

        # redirect user to home page
        flash("Funds adjusted!")
        return redirect(url_for("index"))

    # else if user reached route via GET
    #   (as by clicking a link or via redirect)
    else:
        return render_template("funds.html")
