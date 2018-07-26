import { Component, OnInit, Directive, ViewRef, TemplateRef, ViewContainerRef, ViewChild, ComponentFactoryResolver, ComponentFactory, ComponentRef, ElementRef } from '@angular/core';

@Component({
  selector: 'app-test',
  templateUrl: './test.component.html',
  styleUrls: ['./test.component.css']
})
export class TestComponent implements OnInit {

  constructor() { }

  ngOnInit() {
  }
  xxx: HTMLDivElement;
  public onTest()
  {
    // var elem = document.getElementById("KURT");
    this.xxx = document.getElementById("KURT") as HTMLDivElement;
    // this.xxx.insertAdjacentHTML("afterbegin", "<br>TESTTEST")
    this.xxx.innerHTML=`
    <canvas id="my_canvas" width="400" height="400" style="background-color: aquamarine">
    TESTER
    </canvas>`

    var canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    var context = canvas.getContext("2d");
    context.beginPath();
    context.arc(canvas.width/2, canvas.height/2, 50, 0, Math.PI*2, true);
    context.fill();
    // var b = new HTMLLabelElement();
    // b.textContent = "KURT";
    // elem.appendChild(b);
  }
}
