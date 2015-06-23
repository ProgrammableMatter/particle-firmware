#! /usr/bin/wish
#
#  $Id$
#

package require Itcl
namespace import itcl::*

proc locateInDirs {name dirs} {

  foreach dir ${dirs}  {
     set toTest ${dir}/${name}
     if { [file exists ${toTest}] } {
       return ${toTest}
     }
  }
  error "unable to locate ${name}"
}

#load the avr-simulator package
class Kbd {
    protected {
        variable parent
        variable win
        variable objname
    }

    public {
        constructor { _parent _objname _win } {
            set parent $_parent
            set win $_win
            set objname $_objname

            if { ! [info exists GuiDir] } { set GuiDir "" }
            set prefix "/usr"
            set kbdImage [locateInDirs kbd.xbm \
               [list ${GuiDir} . .. examples \
                  ../. . \
                  ${prefix}/share /usr/share]]
            image create bitmap kbd -file ${kbdImage}

            button $win.b -image kbd
            pack $win.b
            bind $win.b <Enter> "focus $win.b"
            bind $win.b <KeyPress>   "$this SendMakeKey %k"
            bind $win.b <KeyRelease>  "$this SendBrakeKey %k" 
        }

        method SendMakeKey { key } {
            puts "$parent SendToSimulator $objname M$key"
            $parent SendToSimulator $objname M$key
        }

        method SendBrakeKey { key } {
            puts "$parent SendToSimulator $objname B$key"
            $parent SendToSimulator $objname B$key
        }

        
    }
}

class Scope {
    protected {
        variable parent
        variable win
        variable objname
        variable noOfChannels
        variable value
    }

    public {
        constructor { _parent _objname _win _noOfChannels} {
            set parent $_parent
            set win $_win
            set objname $_objname
            set noOfChannels $_noOfChannels


            canvas $win.c
            pack $win.c

        }

        method Redraw { } {
            set l [array names value]
            #puts "l: $l"

            set len [llength $l]

            for { set ii 0 } { $ii < $len } { incr ii } {
                #puts "[lindex $l $ii]"
                set tmp $value([lindex $l $ii])
                #puts "[lindex $l $ii] value: $tmp"
            }
        }

        method ChangeValue { time channel v } {
            puts "Scope Change Value aufgerufen mit $time $channel $v"
            set value($channel,$time) $v
            Redraw
        }

    }
}

class UpdateControl {
    protected {
        variable parent
        variable win
        variable objname
        variable updateOn
    }

    public {
        constructor {  _parent _objname _win } {
            set parent $_parent
            set objname $_objname
            set win $_win
            set updateOn 1

            button .updateControl -text "Update Control" -command "$this ToggleControl" -background green
            pack .updateControl

        }

        method ToggleControl { } {
            if { $updateOn == 1 } {
                set updateOn 0
                .updateControl configure -background red
            } else {
                set updateOn 1
                .updateControl configure -background green
            }

            $parent SendToSimulator UpdateControl $updateOn 
        }
    }
}


class SerialTx {
    protected {
        variable parent
        variable win
        variable objname
    }

    public {
        constructor { _parent _objname _win} {
            set parent $_parent
            set win $_win
            set objname $_objname 

            frame $win.$objname
            

            frame $win.$objname.tx
            text $win.$objname.tx.text -width 40 -height 6
            label $win.$objname.tx.status -text "Serial transmitter of PC"
            label $win.$objname.tx.pin -text "-----------" 

            #bind $win.$objname.tx.text <<Modified>> "puts Hallo"
            bind $win.$objname.tx.text <KeyPress> "$this NewCharEntered %K %A"

            frame $win.$objname.config
            label $win.$objname.config.baudrate -text "-----------"

            pack $win.$objname.tx $win.$objname.tx.text $win.$objname.tx.status $win.$objname.tx.pin 
            pack $win.$objname.config $win.$objname.config.baudrate

            pack $win.$objname


        }

        method NewCharEntered { c a}  {
            #puts "$c   |$a|   " 

            if { "$a" != ""}  {
# Handle any character
              $parent SendToSimulator $objname $a
            } else {
# Handle the special characters
              switch $c {
                     "Return" { set $a "\n"}
              }
              if { "$a" != ""}  {
                $parent SendToSimulator $objname $a
              }
            }
        }

    }

}


class SerialRx {
    protected {
        variable parent
        variable win
        variable objname
    }

    public {
        constructor { _parent _objname _win} {
            set parent $_parent
            set win $_win
            set objname $_objname 


            frame $win.$objname
            
            frame $win.$objname.rx
            text $win.$objname.rx.text -width 40 -height 6
            label $win.$objname.rx.status -text "Serial receiver of PC"
            label $win.$objname.rx.pin -text "-----------"

            frame $win.$objname.config
            label $win.$objname.config.baudrate -text "-----------"

            pack $win.$objname.rx $win.$objname.rx.text $win.$objname.rx.status $win.$objname.rx.pin
            pack $win.$objname.config $win.$objname.config.baudrate

            pack $win.$objname
        }

        method ChangeValue { c } {
            $win.$objname.rx.text insert end $c
        }
    }

}

class Lcd {
    protected {
        variable xsize
        variable ysize
        variable parent
        variable win
        variable objname
    }

    public {
        constructor { _parent _objname _win _xs _ys } {
            set xsize $_xs
            set ysize $_ys
            set parent $_parent
            set win $_win
            set objname $_objname 
            
            #label $win.t -width $xsize -height $ysize
            #pack $win.t

            frame $win.$objname -borderwidth 2 -relief solid
            pack $win.$objname

            for { set y 0 } { $y < $ysize } { incr y } {
                frame $win.$objname.y$y 
                pack $win.$objname.y$y -side top

                for { set x 1 } { $x <= $xsize } { incr x } {
                    #label $win.f.y$y.$x -textvariable $objname$y$x -width 3
                    label $win.$objname.y$y.x$x -text "-" -width 1 
                    pack $win.$objname.y$y.x$x -side left
                }
            }
        }

        method WriteChar { x y ch } {
            set tmp [binary format c $ch]
            #global $objname$y$x
            #set $objname$y$x $tmp

            if { $x <= $xsize } {
                if { $y < $ysize } {
                    $win.$objname.y$y.x$x configure -text $tmp
                }
            }

        }

        method MoveCursor { x y } {
            set cursor_x $x
            incr cursor_x
            set cursor_y $y
            for { set y 0 } { $y < $ysize } { incr y } {
                for { set x 1 } { $x <= $xsize } { incr x } {
                    $win.$objname.y$y.x$x configure -background gray 
                }
            }
            if {$cursor_x < $xsize} {
                if { $cursor_y < $ysize} {
                
                    $win.$objname.y$cursor_y.x$cursor_x configure -background blue 
                }
            }
        }
    }
}

            


class Net {
    protected {
        variable outState
        variable inState
        variable objname
        variable win
        variable parent
    }

    public {
        constructor { _parent _objname _win  } {
            set parent $_parent
            set win $_win
            set objname $_objname

            #puts "Ich bin this: $this extern objname : $objname"

            frame $win.$objname -borderwidth 2 -relief solid
            pack $win.$objname -side left

            label $win.$objname.l -text $objname
            pack $win.$objname.l


            set outState t ; #tritstate
            set inState u  ; #unknown


            menubutton $win.$objname.mb -menu $win.$objname.mb.m -text $outState -relief raised
            menu $win.$objname.mb.m 

            $win.$objname.mb.m add radiobutton -label H -value H -variable $objname.var -command "$this ValueChangedFromGui H"
            $win.$objname.mb.m add radiobutton -label h -value h -variable $objname.var -command "$this ValueChangedFromGui h"
            $win.$objname.mb.m add radiobutton -label t -value t -variable $objname.var -command "$this ValueChangedFromGui t"
            $win.$objname.mb.m add radiobutton -label l -value l -variable $objname.var -command "$this ValueChangedFromGui l"
            $win.$objname.mb.m add radiobutton -label L -value L -variable $objname.var -command "$this ValueChangedFromGui L"
            pack $win.$objname.mb

            global $objname.var
            set $objname.var $outState

            label $win.$objname.lin -text $inState 
            pack $win.$objname.lin
        }

        method ValueChangedFromGui { newValue } {
            set outState $newValue
            $win.$objname.mb configure -text $outState
            #puts "Value from menu $this has changed"
            #puts "$outState"
            $parent SendToSimulator $objname $newValue
        }

        method ChangeValue { newValue } {
            set inState $newValue
            $win.$objname.lin configure -text $inState
        }
    }
}



class AnalogNet {
    protected {
        variable outValue
        variable inValue
        variable objname
        variable win
        variable parent
    }

    public {
        constructor { _parent _objname _win  } {
            set parent $_parent
            set win $_win
            set objname $_objname

            #puts "Ich bin this: $this extern objname : $objname"

            frame $win.$objname -borderwidth 2 -relief solid
            pack $win.$objname -side left

            label $win.$objname.l -text $objname
            pack $win.$objname.l


            set outState 0 ; #tritstate
            set inState 0  ; #unknown

            entry $win.$objname.e -width 4
            pack $win.$objname.e

            button $win.$objname.b -text "Update" -command "$this ValueChangedFromGui"
            pack $win.$objname.b
        }

        method ValueChangedFromGui { } {
            set newValue [$win.$objname.e get]
            #puts "Neuer Wert: $newValue fuer $objname"
            $parent SendToSimulator $objname $newValue
        }

        method ChangeValue { newValue } {
            #set inState $newValue
            #$win.$objname.lin configure -text $inState
        }
    }
}




class Master {
    protected {
       variable sock
    }

    public {
        constructor { _sock } {
            #puts "Constructor from Master called!"
            set sock $_sock 
            #set sock [socket localhost $socketNo]
            fileevent $sock readable "$this ReadFromSimulavr"
            fconfigure $sock -blocking 0
        }

        method ReadFromSimulavr { } {
            if { ![eof $sock] } {
                gets $sock x
                #puts "---> $x <---"

                set readList [ split $x]

                set offset [string first " " $x ]
                set sub [ string range $x $offset end ]

                #puts "offset: $offset    rest: $sub"

               # create class objName parent win objName
                
                set front [lrange $x 1 2] 
                set back [lrange $x 3 end]
                set objName [lindex $x 2]


                switch [lindex $readList 0] {
                    create {
                        #puts "$front $this $objName $back" 
                        eval "$front $this $objName $back" 
                        puts -nonewline $sock "__ack X "
                    }

                    set {
                        #puts "[lindex $readList 1] ChangeValue [lindex $readList 2]" 
                        eval "[lindex $readList 1] ChangeValue [lindex $readList 2]"
                        puts -nonewline $sock "__ack X "
                    }


                    default {
                        #puts ">$x<"
                        eval $x
                        puts -nonewline $sock "__ack X "
                    }
                }

                #puts -nonewline $sock "__ack X "
                flush $sock
                

            } else {
                puts "Error condition in io input handler"
                exit
            }
        }

        method SendToSimulator { id val } {
            #puts "Update the var $id to $val"
            puts -nonewline $sock "$id $val "
            flush $sock
            update
        }
    }
}

# create a server socket 
socket -server NewConnection 7777
    
    
proc NewConnection { _sock _address _port } {
    puts "New Connection on GUI Server established!"
    puts "Socket: $_sock  from Address $_address  and port $_port"
    Master guiMaster_$_sock $_sock
}

    



frame .x
pack .x
#class NEwObjectName (parameters for constructor: baseWindow and NameOfNet)
#Net netz1 guiMaster .x netz1
#Net netz2 guiMaster .x netz2

#Lcd lcd1 guiMaster .x lcd1 20 4


