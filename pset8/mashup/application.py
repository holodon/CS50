import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = \
            "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))


@app.route("/articles")
def articles():
    """Look up articles for geo."""

    geo = request.args.get("geo")

    # check if properly called
    if not geo:
        raise RuntimeError("Missing parameter geo")

    return jsonify(lookup(geo)[:5])
    # or better with no limit:
    # return jsonify(lookup(geo))


@app.route("/search")
def search():
    """Search for places that match query."""

    q = request.args.get("q")

    # check if properly called
    if not q:
        raise RuntimeError("Missing parameter q")

    # check the parameter - allowed are alphanums, space and comma
    if not (re.sub("[\,,\ ]", "", q)).isalnum() or q.count(",") > 2:
        return "[]"

    q = q.strip()
    commas = q.count(",")
    spaces = q.count(" ")

    # quick search for postal_codes
    if q.isdigit():
        rows = db.execute(
            """SELECT * FROM placesFts
            WHERE postal_code MATCH :query
            LIMIT 10""",
            query=q + "*"
        )
        if rows:
            return jsonify(rows)

    # suported comma searches:
    # [postal_code,place_name]+,[postal_code,admin_name1,admin_code1]+,[country_code]*
    elif commas > 0:
        qs = q.split(",")
        q1 = qs[0].strip() + "%"
        q2 = qs[1].strip() + "%"
        if len(qs) == 3:
            q3 = (qs[2].strip() + "%")
        else:
            q3 = "%"
        rows = db.execute(
            """
            SELECT * FROM places WHERE
            (postal_code LIKE :q1 OR place_name LIKE :q1)
            AND (postal_code LIKE :q2 OR admin_name1 LIKE :q2 OR admin_code1 LIKE :q2)
            AND (country_code LIKE :q3)
            LIMIT 10""",
            q1=q1, q2=q2, q3=q3
        )
        if rows:
            return jsonify(rows)

    # searches with up to 2 spaces
    elif spaces < 2:
        rows = db.execute(
            """SELECT * FROM placesFts
            WHERE place_name MATCH :query
            LIMIT 10""",
            query=q + "*"
        )
        if rows:
            return jsonify(rows)

    # no commas and spaces >= 2
    # split, build query and search everywhere
    qs = q.split()
    query = '"' + qs[0].strip() + '*" '
    for i in qs[1:]:
        query = query + '"' + i.strip() + '*" '

    # query the db
    rows = db.execute(
        """SELECT * FROM placesFts
        WHERE placesFts MATCH :query
        GROUP BY place_name, admin_code1, country_code
        ORDER BY RANDOM()
        LIMIT 10""",
        query=query
    )

    return jsonify(rows)


@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search(
            "^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$",
            request.args.get("sw")
            ):
        raise RuntimeError("invalid sw")
    if not re.search(
            "^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$",
            request.args.get("ne")
            ):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute(
            """SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude  <= :ne_lat
            AND (:sw_lng <= longitude  AND longitude  <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng
        )

    else:

        # crosses the antimeridian
        rows = db.execute(
            """SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat
            AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng
        )

    # or with a single (fts) table like:
    # http://stackoverflow.com/a/21236631/6049386
    #        WHERE :sw_lat <= CAST(latitude AS FLOAT)
    # and also MUCH better with:
    # LIMIT 20 or even 30

    # output places as JSON
    return jsonify(rows)
