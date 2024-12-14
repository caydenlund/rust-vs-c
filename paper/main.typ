#let title = [Rust vs. C: A Performance Breakdown]
#let date = [13 December 2024]

#set document(title: title)
#set text(size: 11pt)

#set page(
    paper: "us-letter",
    margin: 1in,
    header: context {
        if counter(page).get().first() > 1 [
            _
            Cayden Lund
            #h(1fr)
            #title
            #h(1fr)
            Page
            #counter(page).display(
                "1 / 1",
                both: true
            )
            _
        ]
    }
)

#align(center)[
    #text(22pt)[
        #title
    ]

    #text(15pt)[
        Cayden Lund (u1182408)
        \ #date
    ]
]

#set heading(numbering: "1.a.i.  ")

#v(2em)

#outline(indent: auto)

#pagebreak()

#include "introduction.typ"

#include "language_fundamentals.typ"

#include "benchmarking-methodology.typ"

#include "benchmark-results.typ"

#include "incomplete-work.typ"

// #include "conclusion.typ"
